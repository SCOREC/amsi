#include "amsiControlService.h"
#include "amsiAssemblable.h"
#include "amsiDataDistribution.h"
#include "amsiMPI.h"
#include "amsiOutput.h"
#include "amsiPlanMigration.h"
#ifdef ZOLTAN
#include <zoltan.h>
#endif
#include <PCU.h>
#include <algorithm>
#include <cstring> //memcpy
#include <fstream>
#include <numeric>
namespace amsi
{
  ControlService * ControlService::instance = NULL;
  size_t ControlService::rel_dist_id_gen = 0;
  ControlService::ControlService()
    : comm_man()
    , task_man()
    , rdd_map()
  {}
  /// @brief Set the CommunicationManager to be used by the AMSI ControlService
  /// @param cm A pointer to the CommunicationManager
  void ControlService::SetCommunicationManager(CommunicationManager * cm)
  {
    comm_man = cm;
  }
  /// @brief Set the TaskManager to be used by the AMSI ControlService
  /// @param tm A pointer to the TaskManager
  void ControlService::SetTaskManager(TaskManager * tm)
  {
    task_man = tm;
  }
  /// @brief Define a CommRelation between two named Tasks
  /// @param nm1 A string containing the name of the sending task
  /// @param nm2 A string containing the name of the recving task
  /// @return size_t An identifier for the CommRelation or 0 if the relation could
  ///                not be created (typically if one of the Tasks doesn't exist).
  size_t ControlService::CommRelation_Define(const std::string& nm1,
                                             const std::string& nm2)
  {
    size_t result = 0;
    size_t t1_id = task_man->getTaskID(nm1);
    size_t t2_id = task_man->getTaskID(nm2);
    if (t1_id != 0 && t2_id != 0)
      result = comm_man->defineRelation(t1_id, t2_id);
    return result;
  }
  void ControlService::setScaleMain(const std::string& scale,
                                    ExecuteFunc function)
  {
    task_man->getTask(scale)->setExecutionFunction(function);
  }
  /// @brief Begin execution of the currently active Task on the local process
  /// @param argc Command line parameter count
  /// @param argv Command line parameter content
  /// @return int Result code for the Task (0 success)
  int ControlService::Execute(int& argc, char**& argv,
                              amsi::Multiscale& multiscale)
  {
    int result = 0;
    // still don't like this method of initiating the inter-comms...
    comm_man->InitInterComms(task_man);
    if (task_man->lockConfiguration()) {
#ifdef ZOLTAN
      float version = 0.0;
      Zoltan_Initialize(0, NULL, &version);
#endif
      result += task_man->Execute(argc, argv, multiscale);
    }
    else
      result = -1;
    return result;
  }
  /// @brief Create a communication plan to communicate data repressented by t1_dd_nm between t1_nm and t2_nm, the distribution of the data on t2_nm once received will be placed into t2_dd_nm.
  /// @param t1_dd_nm The name of the DataDistribution which will be sent across the communication
  /// @param t1_nm The name of the sending task
  /// @param t2_nm The name of the recving task
  /// @return size_t An identifier for the CommPattern or 0 if it was not created
  size_t ControlService::CreateCommPattern(const std::string & t1_dd_nm,
                                           const std::string & t1_nm,
                                           const std::string & t2_nm)
  {
    size_t rdd_id = 0;
    size_t rdd_init_id = 0;
    size_t t1_id = task_man->getTaskID(t1_nm);
    size_t t2_id = task_man->getTaskID(t2_nm);
    // Create additional data distribution for adding data // to support dynamic processes
    Task * tl = task_man->getLocalTask();
    std::string dd_init_nm = t1_dd_nm + "_init";
    createDataDistribution(tl,dd_init_nm);
    if(t1_id && t2_id)
    {
      if(comm_man->CommRelation_Exists(t1_id,t2_id))
      {
        Task * t1 = task_man->getTask(t1_nm);
        Task * t2 = task_man->getTask(t2_nm);
        DataDistribution * t1_dd = t1->getDD(t1_dd_nm);
        Assemble(t1_id,static_cast<Assemblable*>(t1_dd));
        int t1s = taskSize(t1);
        int t2s = taskSize(t2);
        //std::cout << "Generating IDs" << std::endl;
        size_t r_id = comm_man->getRelationID(t1_id,t2_id);
        size_t t1_dd_id = task_man->getTask(t1_nm)->getDD_ID(t1_dd_nm);
        //std::cout << "Combining IDs" << std::endl;
        rdd_id = combine_hashes(r_id,t1_dd_id);
        rdd_map[rdd_id] = std::make_pair(r_id,t1_dd_id);
        // Create additional comm pattern for adding data
        size_t dd_init_id = task_man->DataDist_GetID(t1_nm,dd_init_nm);
        rdd_init_id = combine_hashes(r_id,dd_init_id);
        rdd_map[rdd_init_id] = std::make_pair(r_id,dd_init_id);
        //std::cout << "Going into CommPattern_Create!" << std::endl;
        comm_man->CommPattern_Create(rdd_id,t1_dd,t1s,t2s,&CommPattern_CreateDistro);
        comm_man->CommPattern_CreateRecv(rdd_init_id,t1s,t2s); // This will create a blank comm pattern
        r_init[rdd_id] = rdd_init_id; // Store reference to init comm pattern by original comm pattern
      }
    }
    return rdd_id;
  }
  /// @brief Reconcile a CommPattern object between the ProcessSets associated with the
  ///        Tasks forming the CommRelation on which the pattern is based.
  /// @param rdd_id The identifier of the CommPattern to reconcile
  void ControlService::CommPattern_Reconcile(size_t rdd_id)
  {
    // determine if sender or recver
    std::pair<size_t,size_t> r_dd_id = rdd_map[rdd_id];
    std::pair<size_t,size_t> t_ids = comm_man->Relation_GetTasks(r_dd_id.first);
    Task * tl = task_man->getLocalTask();
    Task * t1 = task_man->Task_Get(t_ids.first);
    Task * t2 = task_man->Task_Get(t_ids.second);
    int t1s = taskSize(t1);
    int t2s = taskSize(t2);
    int task_rank = tl->localRank();
    PCU_Switch_Comm(comm_man->CommRelation_GetInterComm(r_dd_id.first));
    PCU_Comm_Begin();
    if(tl == t1) // sending task
    {
      // retrieve pattern info relating to the current t2 rank
      CommPattern * send_pattern = comm_man->getCommPattern(rdd_id);
      int snd_cnt = getReconcileSendCount(t1s,t2s,task_rank);
      // this might grow too much as the simulation does, could be better to switch to a functional version
      int snd_rnks[snd_cnt];
      getReconcileSendRanks(t1s,t2s,task_rank,&snd_rnks[0]);
      for(int ii = 0; ii < snd_cnt; ii++)
      {
        // extract only nonzero entries and corresponding ranks
        int num_rnks = countRanksSendingTo(send_pattern,snd_rnks[ii]);
        if(num_rnks > 0)
        {
          std::vector<int> rnks(num_rnks);
          std::vector<int> cnts(num_rnks);
          getRanksSendingTo(send_pattern,snd_rnks[ii],&rnks[0]);
          getUnitsSendingTo(send_pattern,snd_rnks[ii],&cnts[0]);
          // intercomm rank of the recving task (hacky)
          int inter_rnk = t1s+snd_rnks[ii];
          int bfr_sz = 1+2*num_rnks;
          int bfr[bfr_sz];
          bfr[0] = num_rnks;
          memcpy(&bfr[1],&rnks[0],sizeof(int)*num_rnks);
          memcpy(&bfr[1+num_rnks],&cnts[0],sizeof(int)*num_rnks);
          PCU_Comm_Write(inter_rnk,&bfr,sizeof(int)*bfr_sz);
        }
      }
      // All processes must call PCU Send and Read, makes this blocking which is bad
      PCU_Comm_Send();
      int rcv_frm = -1;
      void * rcv = NULL;
      size_t rcv_sz = 0;
      while(PCU_Comm_Read(&rcv_frm,&rcv,&rcv_sz)) {}
    }
    else //recving task
    {
      // get the recving comm pattern
      CommPattern * recv_pattern = comm_man->getCommPattern(rdd_id);
      // since only nonzeros are sent the pattern needs to start from zero
      recv_pattern->zero();
      // All processes must call PCU Send
      PCU_Comm_Send();
      int rcv_frm = -1;
      void * rcv= NULL;
      size_t rcv_sz = 0;
      DataDistribution * local_dd = tl->getDD(rdd_dd_map[rdd_id]);
      (*local_dd) = 0;
      while(PCU_Comm_Read(&rcv_frm,&rcv,&rcv_sz))
      {
        int * hdr = (int*)rcv;
        int num_rcv_rnks = hdr[0];
        int * bfr = &hdr[1];
       for(int ii = 0; ii < num_rcv_rnks; ii++)
        {
          int rnk = bfr[ii];
          int cnt = bfr[num_rcv_rnks+ii];
          // using rcv_frm is just as hacky as the t1s+snd_to above, need to map from coupling ranks to task ranks instead (and vice-versa)
          (*recv_pattern)(rnk,task_rank) = cnt;
          (*local_dd) += cnt;
        }
      }
    }
    PCU_Switch_Comm(tl->comm());
  }
  // To allow easy access to comm pattern and task sizes for outputing load info
  void ControlService::getPatternInfo(size_t rdd_id,int & t1s,int & t2s, CommPattern *& cp)
  {
    std::pair<size_t,size_t> r_dd_id = rdd_map[rdd_id];
    std::pair<size_t,size_t> t_ids = comm_man->Relation_GetTasks(r_dd_id.first);
    cp = comm_man->getCommPattern(rdd_id);
    Task * t1 = task_man->Task_Get(t_ids.first);
    Task * t2 = task_man->Task_Get(t_ids.second);
    t1s = taskSize(t1);
    t2s = taskSize(t2);
  }
  size_t ControlService::addData(size_t rdd_id, size_t & cnt)
  {
    std::pair<size_t,size_t> r_dd_ids = rdd_map[rdd_id];
    size_t r_id = r_dd_ids.first;
    size_t dd_id = r_dd_ids.second;
    std::pair<size_t,size_t> t_ids = comm_man->Relation_GetTasks(r_id);
    size_t tsk1_id = t_ids.first;
    //size_t tsk2_id = t_ids.second;
    //
    size_t delta_id = r_init[rdd_id];
    Task * lcl_tsk = task_man->getLocalTask();
    Task * tsk1 = task_man->Task_Get(tsk1_id);
    //Task * tsk2 = task_man->Task_Get(tsk2_id);
    int tsk1_sz = taskSize(tsk1);
    //int tsk2_sz = taskSize(tsk2);
    int tsk_rnk = lcl_tsk->localRank();
    //
    CommPattern * ptrn = comm_man->getCommPattern(rdd_id);
    CommPattern * ptrn_dlta = comm_man->getCommPattern(delta_id);
    ptrn_dlta->zero();
    if(amSender(rdd_id))
    {
      DataDistribution dd_delta(tsk1_sz,tsk_rnk);
      dd_delta = cnt;
      dd_delta.Assemble(lcl_tsk->comm());
      addData_leastFirst(ptrn,ptrn_dlta,&dd_delta);
      DataDistribution * dd = lcl_tsk->getDD(dd_id);
      (*dd) += dd_delta[tsk_rnk];
      CommPattern_Assemble(rdd_id);
      CommPattern_Assemble(delta_id);
      CommPattern_Reconcile(rdd_id);
      CommPattern_Reconcile(delta_id);
    }
    if(amRecver(rdd_id))
    {
      CommPattern_Reconcile(rdd_id);
      CommPattern_Reconcile(delta_id);
      std::vector<int> recv(tsk1_sz);
      // get the units sent to this task rank
      getUnitsSendingTo(ptrn_dlta,tsk_rnk,&recv[0]);
      cnt = std::accumulate(&recv[0],&recv[tsk1_sz-1],0);
    }
    return delta_id;
  }
  bool ControlService::amSender(size_t rdd_id)
  {
    std::pair<size_t,size_t> r_dd_ids = rdd_map[rdd_id];
    std::pair<size_t,size_t> t_ids = comm_man->Relation_GetTasks(r_dd_ids.first);
    Task * lcl_tsk = task_man->getLocalTask();
    Task * snd_tsk = task_man->Task_Get(t_ids.first);
    return lcl_tsk == snd_tsk;
  }
  bool ControlService::amRecver(size_t rdd_id)
  {
    std::pair<size_t,size_t> r_dd_ids = rdd_map[rdd_id];
    std::pair<size_t,size_t> t_ids = comm_man->Relation_GetTasks(r_dd_ids.first);
    Task * lcl_tsk = task_man->getLocalTask();
    Task * snd_tsk = task_man->Task_Get(t_ids.second);
    return lcl_tsk == snd_tsk;
  }
  // This function takes indices of the data to be removed in the vector 'data'
  // It directly alters the communcation pattern specified by rdd_id
  // Using the comm pattern, it sends indices to be removed to the recv task
  //   and fills 'data' accordingly
  // Assumption: The sending task controls the removal
  //   i.e. 'data' acts as input when the sending task calls RemoveData and
  //        acts as output when the receiving task calls RemoveData
  // No load balancing (load balancing when removing data will presumably be
  //   done in a later step using migration)
  void ControlService::RemoveData(size_t rdd_id, std::vector<int> & data)
  {
    std::pair<size_t,size_t> r_dd_id = rdd_map[rdd_id];
    std::pair<size_t,size_t> t_ids = comm_man->Relation_GetTasks(r_dd_id.first);
    Task * tl = task_man->getLocalTask();
    Task * t1 = task_man->Task_Get(t_ids.first);
    Task * t2 = task_man->Task_Get(t_ids.second);
    assert(t1 == tl || t2 == tl);
    int task_rank = tl->localRank();
    int tls = taskSize(tl);
    int recv_from;
    void* recv;
    size_t recv_size;
    PCU_Switch_Comm(comm_man->CommRelation_GetInterComm(r_dd_id.first));
    PCU_Comm_Begin();
    if(tl == t1)
    {
      CommPattern * pattern = comm_man->getCommPattern(rdd_id);
      // Update local part of pattern with removals
      // Record indices of removals
      // Send new pattern count and removal indices
      int ct = 0;
      int num = (*pattern)(task_rank,ct);
      int prevnum = 0;
      unsigned ii = 0;
      bool update = false;
      std::vector<int> indices;
      while(ii<data.size())
      {
        if(data[ii] < num)
        {
          indices.push_back(data[ii]-prevnum);
          (*pattern)(task_rank,ct)--;
          ii++;
          update = true;
        }
        if(data[ii] >= num || ii == data.size())
        {
          if(update)
          {
            std::vector<int> count(1,(*pattern)(task_rank,ct));
            count.insert(count.begin()+1,indices.begin(),indices.end());
            PCU_Comm_Write(tls+ct,count.data(),count.size()*sizeof(int));
            indices.clear();
            update = false;
          }
          ct++;
          prevnum = num;
          num += (*pattern)(task_rank,ct); // TODO: Reading out of bounds?
        }
      }
      PCU_Comm_Send();
      while(PCU_Comm_Read(&recv_from,&recv,&recv_size))
      { }
      // Need to have comm pattern assembled accross this task
      CommPattern_Assemble(rdd_id);
    }
    else if (tl == t2)
    {
      CommPattern * pattern = comm_man->getCommPattern(rdd_id);
      PCU_Comm_Send();
      std::vector<int> recvproc;
      std::vector<int> recvpat;
      while(PCU_Comm_Read(&recv_from,&recv,&recv_size))
      {
        int total = 0;
        for(int ii=0;ii<recv_from;ii++)
          total += (*pattern)(ii,task_rank);
        // Store process that sent and new comm pattern value
        recvproc.push_back(recv_from);
        recvpat.push_back(((int*)(recv))[0]);
        // Convert and store indices
        for(unsigned jj=1;jj<recv_size/sizeof(int);jj++)
          data.push_back( ((int*)(recv))[jj] + total );
      }
      // Update comm pattern
      for(unsigned ii=0;ii<recvproc.size();ii++)
        (*pattern)(recvproc[ii],task_rank) = recvpat[ii];
      // Returned indices may be out of order
    }
    // Switch pcu comms back to task
    PCU_Switch_Comm(tl->comm());
  }
  // assumes that each process have definitive information about the sends it is responsible for
  //template <typename T> // to deal with different commpattern implementations...
  /// @brief Assemble a CommPattern on the ProcessSet of the Task to which it is associated
  /// @param rdd_id The identifier of the CommPattern to assemble
  void ControlService::CommPattern_Assemble(size_t rdd_id)
  {
    CommPattern * to_assemble = comm_man->getCommPattern(rdd_id);
    if(to_assemble != NULL)
    {
      std::pair<size_t,size_t> r_dd_id = rdd_map[rdd_id];
      std::pair<size_t,size_t> tasks = comm_man->Relation_GetTasks(r_dd_id.first);
      Task * st = task_man->Task_Get(tasks.first);
      // only a sending task can assemble
      if(st == task_man->getLocalTask())
      {
        Assemble(tasks.first,static_cast<Assemblable*>(to_assemble));
      }
    }
  }
  /// @brief Create a placeholder CommPattern into which an actual pattern will be
  ///        reconciled. Called by a recving task.
  /// @param t1_dd_nm The name of the data distribution used for the CommPattern
  /// @param t1_nm The name of the sending task in the CommRelation
  /// @param t2_dd_nm The name of the data distribution initialized by the communicated data pattern
  /// @param t2_nm The name of the recving task in the CommRelation
  /// @return size_t The identifier of the placeholder CommPattern created
  size_t ControlService::RecvCommPattern(const std::string & t1_dd_nm,
                                         const std::string & t1_nm,
                                         const std::string & t2_dd_nm,
                                         const std::string & t2_nm)
  {
    size_t rdd_id = 0;
    size_t rdd_init_id = 0;
    size_t t1_id = task_man->getTaskID(t1_nm);
    size_t t2_id = task_man->getTaskID(t2_nm);
    // TODO: should this only be on the sending task?? probably...
    // Create additional data distribution for adding data
    Task * tl = task_man->getLocalTask();
    std::string dd_init_nm = t1_dd_nm + "_init";
    createDataDistribution(tl,dd_init_nm);
    if(t1_id && t2_id)
    {
      if(comm_man->CommRelation_Exists(t1_id,t2_id))
      {
        Task * t1 = task_man->Task_Get(t1_id);
        Task * t2 = task_man->Task_Get(t2_id);
        int t1s = taskSize(t1);
        int t2s = taskSize(t2);
        size_t r_id = comm_man->getRelationID(t1_id,t2_id);
        size_t t1_dd_id = task_man->getTask(t1_nm)->getDD_ID(t1_dd_nm);
        rdd_id = combine_hashes(r_id,t1_dd_id);
        rdd_map[rdd_id] = std::make_pair(r_id,t1_dd_id);
        // Create additional comm pattern for adding data
        size_t dd_init_id = task_man->getTask(t1_nm)->getDD_ID(dd_init_nm);
        rdd_init_id = combine_hashes(r_id,dd_init_id);
        rdd_map[rdd_init_id] = std::make_pair(r_id,dd_init_id);
        //MPI_Comm inter_comm = relation_comms[r_id];
        //MPI_Bcast(&rdd_id,1,MPI_UNSIGNED_LONG,0,inter_comm);
        if(rdd_id)
        {
          comm_man->CommPattern_CreateRecv(rdd_id,t1s,t2s);
          comm_man->CommPattern_CreateRecv(rdd_init_id,t1s,t2s);
          r_init[rdd_id] = rdd_init_id; // Store reference to init comm pattern by original comm pattern
          std::string t2_dd_init_nm = t2_dd_nm + "_init";
          if(!t2->verifyDD(t2_dd_nm))
            createDataDistribution(t2,t2_dd_nm);
          if(!t2->verifyDD(t2_dd_init_nm))
            createDataDistribution(t2,t2_dd_init_nm);
          rdd_dd_map[rdd_id] = t2->getDD_ID(t2_dd_nm);
          rdd_dd_map[rdd_init_id] = t2->getDD_ID(t2_dd_init_nm);
        }
        else
        {} // err
      }
    }
    return rdd_id;
  }
  // Intra task function to plan the migration
  // TODO: Shouldn't use comm pattern, should use data dist
  //
  // Assuming the task is the receiving task in rdd_id
  //
  // This function sends comm_data to be filled by
  //   chosen or provided migration routine.
  // Basically just a wrapper for said function
  void ControlService::planMigration(std::vector<int> & migration_indices,
                                     size_t rdd_id,
                                     int option,
                                     void (*userFunc)())
  {
    // right now this assumes that the task calling is the receiving task in rdd_id
    // this should be VERIFIED and the call should fail if it is not the case
    // Clear migration data
    m_send_to.clear();
    m_index.clear();
    m_t1process.clear();
    m_recv_from.clear();
    std::pair<size_t,size_t> r_dd_id = rdd_map[rdd_id];
    std::pair<size_t,size_t> t_ids = comm_man->Relation_GetTasks(r_dd_id.first);
    Task * tl = task_man->getLocalTask();
    Task * t2 = task_man->Task_Get(t_ids.second);
    int task_rank = tl->localRank();
    CommPattern * pattern = comm_man->getCommPattern(rdd_id);
    DataDistribution * dd = tl->getDD(rdd_dd_map[rdd_id]);
    // Call Plan Migration function specified by the user
    // This function will fill m_send_to and m_index with process number to send to and
    //   local index, respectively, for each piece of migration data to be sent
    // If user indicated a user specified function but did not provide one
    if(option == -1 && userFunc == NULL)
      option = 0;
    switch(option)
    {
    case -2:
      break;
    case -1: // User provided routine
      // userFunc
      break;
    default:
      std::cerr << "Warning: AMSI Plan Migration option not defined - proceeding with default option" << std::endl;
      break;
    case 0:  // Default case is the test case, for now
      CommPattern_PlanMigration_Test(m_index,m_send_to,pattern,dd,task_rank,t2->comm());
      break;
    case 1:
      CommPattern_PlanMigration_Full(m_index,m_send_to,pattern,dd,task_rank,t2->comm());
      break;
#     ifdef ZOLTAN
    case 2:
      CommPattern_PlanMigration_Zoltan(m_index,m_send_to,pattern,dd,task_rank,t2->comm());
      break;
#     endif
    }
    // Return local indices that will be migrated
    migration_indices = m_index;
  }
  /// @brief Invert the specified CommPattern and for use in a reciprocal CommRelation
  /// @param rdd_id The identifier of the CommPattern
  /// @dd_nm The data distribution name for the new CommPattern (should be identical to that of the CommPattern being inverted).
  /// @t1_nm The name of the sending task (for the CommRelation using the inverted CommPattern)
  /// @t2_nm The name of the recving task (for the CommRelation using the inverted CommPattern)
  /// @return size_t The identifier for the inverted CommPattern
  size_t ControlService::CommPattern_UseInverted(size_t rdd_id,
                                                 const std::string & dd_nm,
                                                 const std::string & t1_nm,
                                                 const std::string & t2_nm)
  {
    size_t new_rdd_id = 0;
    // retrieve the old comm pattern
    CommPattern * to_invert = comm_man->getCommPattern(rdd_id);
    if(to_invert != NULL)
    {
      size_t t1_id = task_man->getTaskID(t1_nm);
      size_t t2_id = task_man->getTaskID(t2_nm);
      if(t1_id && t2_id)
      {
        if(comm_man->CommRelation_Exists(t1_id,t2_id))
        {
          //Task * t1 = task_man->getTask(t1_nm);
          //Task * t2 = task_man->getTask(t2_nm);
          //DataDistribution * dd = t1->getDD(dd_nm);
          //int t1s = taskSize(t1);
          //int t2s = taskSize(t2);
          //std::cout << "Generating IDs" << std::endl;
          size_t r_id = comm_man->getRelationID(t1_id,t2_id);
          size_t dd_id = task_man->getTask(t1_nm)->getDD_ID(dd_nm);
          //std::cout << "Combining IDs" << std::endl;
          new_rdd_id = combine_hashes(r_id,dd_id);
          rdd_map[new_rdd_id] = std::make_pair(r_id,dd_id);
          //std::cout << "Going into CommPattern_Create!" << std::endl;
          comm_man->CommPattern_Invert(new_rdd_id,rdd_id);
        }
      }
    }
    return new_rdd_id;
  }
  void ControlService::CommPattern_UpdateInverted(size_t use_rdd_id,size_t create_rdd_id)
  {
    CommPattern * use_pattern = comm_man->getCommPattern(use_rdd_id);
    CommPattern * create_pattern = comm_man->getCommPattern(create_rdd_id);
    std::pair<size_t,size_t> r_dd_id = rdd_map[use_rdd_id];
    std::pair<size_t,size_t> t_ids = comm_man->Relation_GetTasks(r_dd_id.first);
    Task * t1 = task_man->Task_Get(t_ids.first);
    Task * t2 = task_man->Task_Get(t_ids.second);
    int t1s = taskSize(t1);
    int t2s = taskSize(t2);
    for(int ii = 0; ii < t1s; ii++)
      for(int jj = 0; jj < t2s; jj++)
        (*create_pattern)(jj,ii) = (*use_pattern)(ii,jj);
  }
  void ControlService::Assemble(size_t t_id, Assemblable * control_data)
  {
    Task * t = task_man->Task_Get(t_id);
    MPI_Comm task_comm = t->comm();
    control_data->Assemble(task_comm);
  }
  void ControlService::Reconcile(size_t r_id, Reconcilable * control_data)
  {
    //std::pair<size_t,size_t> t_ids = comm_man->Relation_GetTasks(r_id);
    MPI_Comm inter_comm = comm_man->CommRelation_GetInterComm(r_id);
    control_data->Reconcile(inter_comm);
  }
} // namespace amsi
