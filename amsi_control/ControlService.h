#ifndef CONTROLSERVICE_AMSI_H_
#define CONTROLSERVICE_AMSI_H_
#include "amsiInterfaceConfig.h"
#include "amsiMetaConfig.h"
#include "Assemblable.h"
#include "CommPattern.h"
#include "CommunicationManager.h"
#include "DataDistribution.h"
#include "TaskManager.h"
#include "amsiUtilFunctions.h"
#include "amsiMPI.h"
#include <cstring>
#include <list>
#include <string>
namespace amsi
{
    /// Class decleration of the ControlService class (set to be renamed as MultiscaleManager or ScaleCouplingManager or...),
    ///  used as the top-level interface to the task managing portions of the AMSI infrastructure.
    class ControlService
    {
    public:
      static ControlService * Instance()
      { return instance = instance == NULL ? new ControlService() : instance; };

      void SetCommunicationManager(CommunicationManager*);
      void SetTaskManager(TaskManager*);
      TaskManager* GetTaskManager() { return task_man; }
      CommunicationManager * getCommunicationManager() {return comm_man;}

      void setScaleMain(const std::string &, ExecuteFunc);
      int Execute(int& argc,char **& argv);

      size_t CommRelation_Define(const std::string & nm1,
                                 const std::string & nm2);

      size_t CommRelation_GetID(const std::string & nm1,
                                const std::string & nm2);

      size_t CreateCommPattern(const std::string & t1_dd,
                               const std::string & t1,
                               const std::string & t2);

      void getPatternInfo(size_t rdd_id,int &,int &,CommPattern *&);

      // Remove and add data from comm pattern
      void RemoveData(size_t rdd_id, std::vector<int> & data);

      template <typename D, template <typename T, typename All = std::allocator<T> > class Container>
      size_t AddData(size_t rdd_id,
                     Container<D> & objects,
                     std::vector<int> & data,
                     int option = 0,
                     void (*userFunc)() = NULL );

      // Migration functions
      void planMigration(std::vector<int> & migration_indices,
                         size_t rdd_id,
                         int option = 0,
                         void (*userFunc)() = NULL);

      template <typename D, template<typename T, typename All = std::allocator<T> > class Container>
      void migration(std::vector< std::vector<char> > & migration_data,
                     size_t rdd_id,
                     Container<D> & objects);



      template <typename D, template <typename T, typename All = std::allocator<T> > class Container>
      void shareMigration(size_t rdd_id, Container<D> & objects);

      void CommPattern_Assemble(size_t rdd_id);

      size_t RecvCommPattern(const std::string & t1_dd_nm,
                             const std::string & t1_nm,
                             const std::string & t2_nm,
                             const std::string & t2_dd_nm);

      void CommPattern_Reconcile(size_t rdd_id);

      size_t CommPattern_UseInverted(size_t rdd_id,
                                     const std::string & dd,
                                     const std::string & t1,
                                     const std::string & t2);

      void CommPattern_UpdateInverted(size_t use_rdd_id,size_t create_rdd_id);

      void Assemble(size_t,Assemblable*);
      void Reconcile(size_t,Reconcilable*);

      template <typename D>
        void Assemble(size_t t_id, D & data, MPI_Datatype type);

      template <typename D>
        void Reconcile(size_t r_id, D & data, MPI_Datatype type);

      template <typename D>
        void Relation_Broadcast(size_t r_id, D & data, MPI_Datatype type);

      template <typename D, template <typename T,typename All = std::allocator<T> > class Container>
        void Communicate(size_t rdd_id,Container<D> & buffer,MPI_Datatype type);

    private:
      ControlService();
      ControlService(const ControlService &);
      ControlService & operator=(const ControlService &);

      static ControlService * instance;
      static size_t rel_dist_id_gen;

      CommunicationManager * comm_man;
      TaskManager * task_man;


      // map that stores the commpattern -> relation, data_distribution information
      typedef std::map<size_t, std::pair<size_t,size_t> > rddmap_t;
      rddmap_t rdd_map;

      // map that stores the commpattern -> recv_data_distribution information
      typedef std::map<size_t, size_t> rdd_dd_map_t;
      rdd_dd_map_t rdd_dd_map;

      typedef std::map<size_t,size_t> r_init_t;
      r_init_t r_init;


      // Temp location for migration data
      std::vector<int> m_send_to;
      std::vector<int> m_index;
      std::vector<int> m_t1process;
      std::vector<int> m_recv_from;

    };


    template <typename D>
      void ControlService::Assemble(size_t t_id, D & data, MPI_Datatype type)
    {
      Task * t = task_man->Task_Get(t_id);
      MPI_Comm task_comm = t->comm();
      int task_rank = t->localRank();
      int data_size = data.NumPerRank();

      MPI_Allgather(&data[task_rank],data_size,type,
                    &data[0],data_size,type,
                    task_comm);
    }

    // need to differentiate between 1-to-all and many-to-all Reconciliations
    template <typename D>
      void ControlService::Reconcile(size_t r_id, D & data, MPI_Datatype type)
    {
      // determine if sender of recver
      std::pair<size_t,size_t> t_ids = comm_man->Relation_GetTasks(r_id);

      Task * lt = task_man->getLocalTask();

      Task * t1 = task_man->Task_Get(t_ids.first);
      Task * t2 = task_man->Task_Get(t_ids.second);

      int t1s = taskSize(t1);
      int t2s = taskSize(t2);

      int t2_per_t1 = t2s / t1s;
      int extra_t2 = t2s % t1s;

      int task_rank = lt->localRank();

      if(lt == t1) // sending task
      {
        // make sure the data to be reconciled is assembled on the local task
        Assemblable * is_assemblable = dynamic_cast<Assemblable*>(&data);
        if(is_assemblable != NULL)
        {
          if(is_assemblable->isAssembled() != true)
            Assemble(t_ids.first,is_assemblable); //built-in assemble
        }
        else
          Assemble(t_ids.first,data,type); // user-type assemble

        int to_send = t2_per_t1 + (task_rank < extra_t2);

        for(int ii = 0; ii < to_send; ii++)
        {
          int send_to = task_rank + (ii * t1s);
            t_ssend(data[send_to],type,t2->localToGlobalRank(send_to));
        }
      }
      else // recving task
      {
        int recv_from = task_rank % t1s;
        t_srecv(data,type,t1->localToGlobalRank(recv_from));
      }
    }

    // need to differentiate between 1-to-all and many-to-all Reconciliations
    template <typename D>
      void ControlService::Relation_Broadcast(size_t r_id, D & data, MPI_Datatype type)
    {
      // determine if sender of recver
      std::pair<size_t,size_t> t_ids = comm_man->Relation_GetTasks(r_id);

      Task * lt = task_man->getLocalTask();

      Task * t1 = task_man->Task_Get(t_ids.first);
      Task * t2 = task_man->Task_Get(t_ids.second);

      int t1s = taskSize(t1);
      int t2s = taskSize(t2);

      int t2_per_t1 = t2s / t1s;
      int extra_t2 = t2s % t1s;

      int task_rank = lt->localRank();

      if(lt == t1) // sending task
      {
        int to_send = t2_per_t1 + (task_rank < extra_t2);

        for(int ii = 0; ii < to_send; ii++)
        {
          int send_to = task_rank + (ii * t1s);
            t_ssend(data,type,t2->localToGlobalRank(send_to));
        }
      }
      else // recving task
      {
        int recv_from = task_rank % t1s;
        t_srecv(data,type,t1->localToGlobalRank(recv_from));
      }
    }

    /// @brief Templated member function used to communicate all data related to a specific CommPattern,
    ///         this call is collective on the union of the ProcessSets associated with the sending and
    ///         recving task-groups.
    /// @tparam D The plain-old-data datatype to be communicated.
    /// @tparam Cont The container-class type of the storage object holding a contiguous buffer of data
    ///              (with the first piece of data being retrievable using the bracket [] operators and
    ///              reference operator &)
    /// @tparam Alloc The allocator needed by the container template class to allocate and deallocate
    ///               space for data (required to work with STL data storage template classes).
    /// @param rdd_id The unique id for the CommPattern defining this communication action.
    /// @param buffer The Cont<D,Alloc> object containing the data to be sent or into which data will
    ///               be recved.
    /// @param type An MPI_Datatype describing the POD objects to be sent/recved in this communication
    ///             action.
    template <typename D, template <typename T, typename All = std::allocator<T> > class Container>
      void ControlService::Communicate(size_t rdd_id,
                                       Container<D> & buffer,
                                       MPI_Datatype type)
      {
        // determine if sender or recver
        std::pair<size_t,size_t> r_dd_id = rdd_map[rdd_id];
        std::pair<size_t,size_t> t_ids = comm_man->Relation_GetTasks(r_dd_id.first);
        Task * tl = task_man->getLocalTask();
        int task_rank = tl->localRank();
        Task * t1 = task_man->Task_Get(t_ids.first); // sending task
        Task * t2 = task_man->Task_Get(t_ids.second); // recving task
        assert(t1 == tl || t2 == tl);
        int t1s = taskSize(t1);
        // Get type size
        int tp_sz = 0;
        MPI_Type_size(type,&tp_sz);
#       ifdef CORE
        PCU_Switch_Comm(comm_man->CommRelation_GetInterComm(r_dd_id.first));
        PCU_Comm_Begin();
#       endif
        if(tl == t1) // if the local task is the sending task
        {
          unsigned int local_count = t1->getLocalDDValue(r_dd_id.second);
          CommPattern * send_pattern = comm_man->getCommPattern(rdd_id);
          int num_snt_frm = countRanksSentFrom(send_pattern,task_rank);
          std::vector<int> snt_rnks(num_snt_frm);
          std::vector<int> snt_cnts(num_snt_frm);
          getRanksSentFrom(send_pattern,task_rank,&snt_rnks[0]);
          getUnitsSentFrom(send_pattern,task_rank,&snt_cnts[0]);
          size_t offset = 0;
#         ifndef CORE
          buffer_offset<D> bo;
          bo.buffer = &buffer[0];
#         endif
          for(unsigned int ii = 0; ii < num_snt_frm; ii++)
          {
#           ifdef CORE
            int inter_rnk = t1s+snt_rnks[ii]; // hacky and awful
            PCU_Comm_Write(inter_rnk,
                           &buffer[offset],
                           snt_cnts[ii]*tp_sz);
#           else
            bo.s = to_send;
            bo.offset = offset;
            t_send(bo,type,t2->localToGlobalRank(ii));
#           endif
            offset += snt_cnts[ii];
          }
#         ifdef CORE
          PCU_Comm_Send();
          int recvfrom = -1;
          void * recv;
          size_t recv_size;
          while(PCU_Comm_Read(&recvfrom,&recv,&recv_size))
          { }
#         endif
        }
        else if (t2 == tl)// recv'ing
        {
#         ifdef CORE
          PCU_Comm_Send();
          size_t bfr_sz = 0;
          size_t bfr_hd = 0;
          size_t rcv_sz = 0;
          int rcv_frm = -1;
          void * rcv = NULL;
          while(PCU_Comm_Read(&rcv_frm,&rcv,&rcv_sz))
          {
            size_t rcv_cnt = rcv_sz / tp_sz;
            bfr_sz += rcv_cnt;
            buffer.resize(bfr_sz); // this is stupid and awful
            memcpy(&buffer[bfr_hd],rcv,rcv_sz);
            bfr_hd += rcv_cnt;
          }
#         else
          recv_buffer_offset<D, Container> buffer_offset;
          buffer_offset.buffer = &buffer;
          buffer_offset.offset = 0;
          for(size_t ii = 0; ii < recv_from.size(); ii++)
            {
              int to_recv = recv_from[ii];
              {
                if(to_recv > 0)
                  buffer_offset.offset += t_recv(buffer,type,t1->localToGlobalRank(ii));
              }
            }
#         endif
        }

#       ifdef CORE
        // Switch pcu comms back to task
        PCU_Switch_Comm(tl->comm());
#       endif
      }


    // This function adds data while load balancing
    // Assumption: Sending task specifies additions
    // Sending task process:
    //   - Contents of 'data' doesn't matter, only size is used (size is number of added data)
    //   - rdd_id specifies the comm pattern
    //   - 'objects' is a vector of references to the data objects corresponding to
    //     the data distribution on the sending task process. references to the added data
    //     should be at the end
    //   - AMSI reorders 'objects' based on load balancing, assuming no migration of previous data
    //     i.e. any previous data must stay in current location within comm pattern
    //   - Sending task process can then use 'objects' to compile data to send via this
    //     comm pattern
    // Receiving task process:
    //   - 'data' will be filled by this function and will contain indices to place data before
    //   - rdd_id specifies the comm pattern
    //   - 'objects' is not used
    template <typename D, template <typename T, typename All = std::allocator<T> > class Container>
    size_t ControlService::AddData(size_t rdd_id,
                                   Container<D> & objects,
                                   std::vector<int> & data,
                                   int option,
                                   void (*userFunc)())
    {
      std::pair<size_t,size_t> r_dd_id = rdd_map[rdd_id];
      std::pair<size_t,size_t> t_ids = comm_man->Relation_GetTasks(r_dd_id.first);

      size_t delta_id = r_init[rdd_id]; // Get init comm pattern reference

      Task * tl = task_man->getLocalTask();
      Task * t1 = task_man->Task_Get(t_ids.first);
      Task * t2 = task_man->Task_Get(t_ids.second);

      int t1s = taskSize(t1);
      int t2s = taskSize(t2);
      int task_rank = tl->localRank();

      // If user indicated a user specified load balancing function but did not provide one
      if(option == -1 && userFunc == NULL)
        option = 0;

      // If sending task
      if(tl == t1)
      {
        int numNewData[t1s];
        numNewData[task_rank] = data.size();
        CommPattern * pattern = comm_man->getCommPattern(rdd_id);
        CommPattern * delta_pattern = comm_man->getCommPattern(delta_id);
        // Zero out delta comm pattern
        for(int ii=0;ii<t1s;ii++)
          for(int jj=0;jj<t2s;jj++)
            (*delta_pattern)(ii,jj) = 0;

        // exchange numNewData among all sending task processes
        int tempData = numNewData[task_rank]; // On Q the send and recv buffers need to be different
        MPI_Allgather(&tempData,1,MPI_INT,&numNewData[0],1,MPI_INT,t1->comm());

        // Call function to update both the original comm pattern and init comm pattern
        // based on numNewData (this function will actually do the load balancing)
        switch(option)
        {
        case -1: // User specified function
          // userFunc
          break;
        default:
          std::cerr << "Warning: AMSI Load Balancing option not defined - proceeding with default option" << std::endl;
        case 0:
          CommPattern_LoadBalance_Spread(pattern,delta_pattern,&numNewData[0]);
          break;
        case 1:
          CommPattern_LoadBalance_LeastFirst(pattern,delta_pattern,&numNewData[0]);
          break;
        case 2:
          CommPattern_LoadBalance_Test(pattern,delta_pattern,&numNewData[0],task_rank);
          break;
        }
        CommPattern_Assemble(rdd_id);
        CommPattern_Assemble(delta_id);

        // Reorder objects vector based on comm patterns (Need to fill data indices? Probably not)
        // Trying to make something that will work for both vectors and lists...
        int numOldData = objects.size() - data.size();
        Container<D> tempObj;
        typename Container<D>::iterator oldObj = objects.begin();
        typename Container<D>::iterator newObj = objects.begin();
        // Move newObj to portion of objects containing new objects...
        for(int ii=0;ii<numOldData;ii++)
          newObj++;
        // Fill tempObj
        for(int ii=0;ii<t2s;ii++)
        {
          int numOldDataElement = (*pattern)(task_rank,ii) - (*delta_pattern)(task_rank,ii);
          int numNewDataElement = (*delta_pattern)(task_rank,ii);
          for(int jj=0;jj<numOldDataElement;jj++)
          {
            tempObj.push_back(*oldObj);
            oldObj++;
          }
          for(int jj=0;jj<numNewDataElement;jj++)
          {
            tempObj.push_back(*newObj);
            newObj++;
          }
        }
        // Clear and refill objects to return
        objects.clear();
        for(newObj = tempObj.begin(); newObj != tempObj.end(); ++newObj)
          objects.push_back(*newObj);

        // Reconcile orig and init comm pattern on recv task
        CommPattern_Reconcile(rdd_id);
        CommPattern_Reconcile(delta_id);
      }
      else
      {
        // Get the new orig and init comm patterns
        CommPattern_Reconcile(rdd_id);
        CommPattern_Reconcile(delta_id);

        // Fill data vector with appropriate indices
        CommPattern * pattern = comm_man->getCommPattern(rdd_id);
        CommPattern * delta_pattern = comm_man->getCommPattern(delta_id);
        for(int ii = 0; ii < t1s; ii++)
        {
          int nw = (*delta_pattern)(ii,task_rank);
          for(int jj = 0; jj < nw; jj++)
            data.push_back(0);
        }
      }
      return delta_id;
    }


    // Intra task function which does acutal communication for
    //   migration according to migration variables which should
    //   have been set in planMigration
    // Input parameters:
    //  - migration_data - A vector of buffers containing data for each
    //    object being migrated away from this process
    //  - rdd_id - Comm pattern for migration
    //  - objects - List of multiscale data elements
    // Output parameters:
    //  - migration_data - Contains data for any objects which have
    //    migrated to this process
    //  - objects - The list of multiscale data elements has been
    //    updated to reflect the migration. A NULL element has been
    //    added for each object which migrated to this process
    //
    // TODO Would really want to not have to reference a particular comm pattern
    //   for this function, might need to split this function up eventually. One part
    //   which does migration without referencing comm pattern and just place migration
    //   data at the front of the list. Then a second function (part of shareMigration?)
    //   which actually reorders the objects in terms of the comm pattern
    template <typename D, template <typename T, typename All = std::allocator<T> > class Container>
    void ControlService::migration(std::vector< std::vector<char> > & migration_data,
                                   size_t rdd_id,
                                   Container<D> & objects)
    {
      std::pair<size_t,size_t> r_dd_id = rdd_map[rdd_id];
      std::pair<size_t,size_t> t_ids = comm_man->Relation_GetTasks(r_dd_id.first);

      Task * tl = task_man->getLocalTask();
      Task * t1 = task_man->Task_Get(t_ids.first);
      Task * t2 = task_man->Task_Get(t_ids.second);

      int t1s = taskSize(t1);
      int t2s = taskSize(t1);
      int task_rank = tl->localRank();

      MPI_Comm task_comm = t2->comm();

      int recv_from;
      void* recv;
      size_t recv_size;

#     ifdef CORE
      PCU_Switch_Comm(task_comm);
      PCU_Comm_Begin();
#     endif

      CommPattern * pattern = comm_man->getCommPattern(rdd_id);

      // ***** SEND MIGRATION DATA ***** //

      // Loop over task 1 processes to determine placing in implicit
      //   ordering and corresponding "task 1" process
      // Assumption: migration data contents are ordered by local indices (m_index)
      int current_data = 0;
      int current_send = 0;
      for(int ii = 0; ii < t1s; ii++)
      {
        for(int jj = 0; jj < (*pattern)(ii,task_rank); jj++)
        {
          if(current_send >= m_send_to.size())
            break;

          // Send migration data
          if(current_data == m_index[current_send])
          {
            // Add task 1 process and element index (jj) to migration data,
            //   so that recv process knows where it was taken from, in order
            //   to inform companion task of the swap
            // "element index" means the index within a particular element of a comm pattern matrix
            migration_data[current_send].resize( migration_data[current_send].size() + 2 * sizeof(int) );
            memcpy( migration_data[current_send].data() + migration_data[current_send].size() - 2*sizeof(int),
                    &ii,
                    sizeof(int) );
            memcpy( migration_data[current_send].data() + migration_data[current_send].size() - sizeof(int),
                    &jj,
                    sizeof(int) );
#           ifdef CORE
            PCU_Comm_Write(m_send_to[current_send],
                           migration_data[current_send].data(),
                           migration_data[current_send].size());
#           else

#           endif
            current_send++;

          }

          current_data++;
        }

      }
#     ifdef CORE
      PCU_Comm_Send();
#     endif

      // ***** UPDATE COMM PATTERN ***** //

      // Update local comm pattern with removed data
      // When inserting objects later, we need to reference
      //   the comm pattern with migrated data subtracted out
      // Later, in shareMigration, the comm pattern will be
      //   updated by the corresponding task and reconciled on
      //   this task

      // the above explanation is weak, it will need to be made more clear
      int offset[t1s];
      offset[0] = (*pattern)(0,task_rank);
      for(int ii=1;ii<t1s;ii++)
        offset[ii] = offset[ii-1] + (*pattern)(ii,task_rank);

      for(int ii=0;ii<m_send_to.size();ii++)
        for(int jj=0;jj<t1s;jj++)
          if(m_index[ii] < offset[jj])
          {
            (*pattern)(jj,task_rank)--;
            break;
          }

      DataDistribution * dd = tl->getDD(rdd_dd_map[rdd_id]);
      (*dd)[task_rank] -= m_send_to.size();

      migration_data.clear();
      m_index.clear();
      m_send_to.clear();

      // ***** RECEIVE MIGRATION DATA ***** //
#     ifdef CORE
      int kk = 0;
      while(PCU_Comm_Read(&recv_from,&recv,&recv_size))
      {
        // Get ints off of end
        int t1process;
        int elem_index;
        memcpy(&t1process, (char*)recv + recv_size - 2*sizeof(int), sizeof(int));
        memcpy(&elem_index, (char*)recv + recv_size - sizeof(int), sizeof(int));

        // Store all migration info
        m_t1process.push_back(t1process);
        m_index.push_back(elem_index);
        m_recv_from.push_back(recv_from);

        // Store migration data
        migration_data.resize(kk+1);
        migration_data[kk].resize(recv_size - 2*sizeof(int));
        memcpy(migration_data[kk].data(), recv, recv_size - 2*sizeof(int));
        kk++;
      }
#     endif

      // ***** REORDER MIGRATION DATA AND INSERT OBJECTS ***** //

      if(migration_data.size() > 0)
      {

        // Create new vectors/lists of all migration data and multiscale data
        std::vector< std::vector<char> > temp_md;
        std::vector<int> temp_index;
        std::vector<int> temp_t1p;
        std::vector<int> temp_rf;
        Container<D> temp_objects;
        typename Container<D>::iterator currentObject = objects.begin();

        // Order data by task 1 process
        for(int ii=0;ii<t1s;ii++)
        {
          for(int jj=0;jj<migration_data.size();jj++)
            if(m_t1process[jj] == ii)
            {
              temp_md.push_back(migration_data[jj]);
              temp_index.push_back(m_index[jj]);
              temp_t1p.push_back(m_t1process[jj]);
              temp_rf.push_back(m_recv_from[jj]);
              temp_objects.push_back(NULL);
            }

          for(int jj=0;jj<(*pattern)(ii,task_rank);jj++)
          {
            temp_objects.push_back(*currentObject);
            currentObject++;
          }
        }

        // Reconstruct all the lists
        migration_data = temp_md;
        m_t1process = temp_t1p;
        m_index = temp_index;
        m_recv_from = temp_rf;
        objects = temp_objects;

        (*dd)[task_rank] +=  migration_data.size();
      }

      dd->Assemble(task_comm);

    }


    // Inter task function to inform companion task of migration
    //   and update the comm pattern
    //
    // Sending task (according to rdd_id): This task actually does the
    //   receiving in this task. Receives data about what migration occured
    //   on the receiving task. Reorders object list accordingly. Also
    //   updates the comm pattern and reconciles.
    // - rdd_id - comm pattern
    // - objects - List or vector of data corresponding to rdd_id which
    //   will be reordered
    //
    // Receiving task (according to rdd_id): This task sends migration data
    //   to the sending task. Then waits to reconcile the comm pattern.
    // - rdd_id - comm pattern
    // - objects - dummy parameter
    template <typename D, template <typename T, typename All = std::allocator<T> > class Container>
    void ControlService::shareMigration(size_t rdd_id, Container<D> & objects)
    {
      std::pair<size_t,size_t> r_dd_id = rdd_map[rdd_id];
      std::pair<size_t,size_t> t_ids = comm_man->Relation_GetTasks(r_dd_id.first);

      Task * tl = task_man->getLocalTask();
      int task_rank = tl->localRank();

      Task * t1 = task_man->Task_Get(t_ids.first); // sending task
      Task * t2 = task_man->Task_Get(t_ids.second); // recving task

      int t1s = taskSize(t1);
      int t2s = taskSize(t2);

#     ifdef CORE
      int recv_from;
      void* recv;
      size_t recv_size;

      PCU_Switch_Comm(comm_man->CommRelation_GetInterComm(r_dd_id.first));
      PCU_Comm_Begin();
#     endif

      CommPattern * pattern = comm_man->getCommPattern(rdd_id);

      // If receiver in comm pattern rdd_id, then send
      //   info to corresponding task 1 processes
      if(tl == t2)
      {
        int numMigration = m_t1process.size();
        for(int ii=0;ii<numMigration;ii++)
        {
          int dataToSend[2];
          dataToSend[0] = m_index[ii]; // 'old' pattern element index
          dataToSend[1] = m_recv_from[ii];   // old task 2 process

#         ifdef CORE
          PCU_Comm_Write(m_t1process[ii],dataToSend,2*sizeof(int));
#         else

#         endif
        }

#       ifdef CORE
        PCU_Comm_Send();
        while(PCU_Comm_Read(&recv_from,&recv,&recv_size))
        { }
#       endif

        // Get updated comm pattern from task 1
        CommPattern_Reconcile(rdd_id);

      }

      // If sender in comm pattern rdd_id, then receive
      //   info about migration that occured on task 2
      //   Update object list accordingly
      else // (tl == t1)
      {

        std::vector<int> newT2proc;
        std::vector<int> oldT2proc;
        std::vector<int> oldCompIndex;

#       ifdef CORE
        PCU_Comm_Send();

        while(PCU_Comm_Read(&recv_from,&recv,&recv_size))
        {
          newT2proc.push_back(recv_from - t1s);
          oldCompIndex.push_back( ((int*)recv)[0] );
          oldT2proc.push_back( ((int*)recv)[1] );
        }
#       endif

        int numMigration = newT2proc.size();
        Container<D> tempObj;
        typename Container<D>::iterator currentObj = objects.begin();

        // Re construct object list/vector from old list and migration lists
        int index = 0;
        for(int ii=0;ii<t2s;ii++)
        {

          // Search for data that was migrated to corresponding task 2 process
          for(int jj=0;jj<numMigration;jj++)
          {
            // If one moves here find where it came from
            if(ii == newT2proc[jj])
            {
              typename Container<D>::iterator cur_obj = objects.begin();
              for(int kk=0;kk<t2s;kk++)
              {
                if(kk == oldT2proc[jj])
                {
                  for(int nn=0;nn<oldCompIndex[jj];nn++)
                    cur_obj++;
                  tempObj.push_back(*cur_obj); // Add to new position in list from old position
                  break;
                }
                else
                  for(int nn=0;nn<(*pattern)(task_rank,kk);nn++)
                    cur_obj++;
              }
            }
          }

          // Place remaining data from old list UNLESS it's been sent somewhere else
          //   in which case it was (or will be) added by above for loop
          for(int jj=0;jj<(*pattern)(task_rank,ii);jj++)
          {
            bool toAdd = true;
            for(int kk=0;kk<numMigration;kk++)
              if(oldT2proc[kk] == ii && oldCompIndex[kk] == jj)
                toAdd = false;

            if(toAdd)
              tempObj.push_back(*currentObj);

            currentObj++;

          }
        }

        // Reset object list/vector from newly constructed version
        objects = tempObj;

        //std::cout << "Before update: "  << std::endl << (*pattern) << std::endl;
        // Update, assemble, and reconcile comm pattern
        for(int ii=0;ii<numMigration;ii++)
        {
          (*pattern)(task_rank,oldT2proc[ii])--;
          (*pattern)(task_rank,newT2proc[ii])++;
        }
        CommPattern_Assemble(rdd_id);
        CommPattern_Reconcile(rdd_id);
        //std::cout << "After update: "  << std::endl << (*pattern) << std::endl;
      }

      // Switch pcu comms back to task
#     ifdef CORE
      PCU_Switch_Comm(tl->comm());
#     endif
    }






} // namespace amsi

#endif
