#include <PCU.h>
namespace amsi
{
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
  // assumes that the data has been assembled locally
  template <typename T>
    void ControlService::scaleBroadcast(size_t r_id, T * buf)
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
    if(lt == t1) // send
    {
      int to_send = t2_per_t1 + (task_rank < extra_t2);
      for(int ii = 0; ii < to_send; ii++)
      {
        int send_to = task_rank + (ii * t1s);
        amsi::send(buf,t2->localToGlobalRank(send_to),1,AMSI_COMM_WORLD);
      }
    }
    else // recv
      amsi::recv(buf,MPI_ANY_SOURCE,1,AMSI_COMM_WORLD);
  }
  template <typename I, typename O>
    void ControlService::aSendBroadcast(O out, size_t rid, I * bfr, size_t cnt)
  {
    std::pair<size_t,size_t> t_ids = comm_man->Relation_GetTasks(rid);
    Task * t1 = task_man->Task_Get(t_ids.first);
    Task * t2 = task_man->Task_Get(t_ids.second);
    int t1s = taskSize(t1);
    int t2s = taskSize(t2);
    int t2_per_t1 = t2s / t1s;
    int extra_t2 = t2s % t1s;
    int task_rank = t1->localRank();
    int to_send = t2_per_t1 + (task_rank < extra_t2);
    for(int ii = 0; ii < to_send; ii++)
    {
      int send_to = task_rank + (ii * t1s);
      *out = amsi::asend(bfr,t2->localToGlobalRank(send_to),cnt,AMSI_COMM_WORLD);
      ++out;
    }
  }
  template <typename T>
    int ControlService::aRecvBroadcastSize(size_t rid)
  {
    std::pair<size_t,size_t> t_ids = comm_man->Relation_GetTasks(rid);
    Task * t1 = task_man->Task_Get(t_ids.first);
    Task * t2 = task_man->Task_Get(t_ids.second);
    int t1s = taskSize(t1);
    int scl_rnk = t2->localRank();
    int rcv_frm = t1->localToGlobalRank(scl_rnk % t1s);
    return arecv_sz<T>(rcv_frm,AMSI_COMM_WORLD);
  }
  template <typename I, typename O>
    void ControlService::aRecvBroadcast(O out, size_t rid, I * bfr, size_t cnt)
  {
    std::pair<size_t,size_t> t_ids = comm_man->Relation_GetTasks(rid);
    Task * t1 = task_man->Task_Get(t_ids.first);
    Task * t2 = task_man->Task_Get(t_ids.second);
    int t1s = taskSize(t1);
    int scl_rnk = t2->localRank();
    int rcv_frm = t1->localToGlobalRank(scl_rnk % t1s);
    *out = amsi::arecv(bfr,rcv_frm,cnt,AMSI_COMM_WORLD);
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
      PCU_Switch_Comm(comm_man->CommRelation_GetInterComm(r_dd_id.first));
      PCU_Comm_Begin();
      if(tl == t1) // if the local task is the sending task
      {
        //unsigned local_count = t1->getLocalDDValue(r_dd_id.second);
        CommPattern * send_pattern = comm_man->getCommPattern(rdd_id);
        unsigned num_snt_frm = countRanksSentFrom(send_pattern,task_rank);
        std::vector<int> snt_rnks(num_snt_frm);
        std::vector<int> snt_cnts(num_snt_frm);
        getRanksSentFrom(send_pattern,task_rank,&snt_rnks[0]);
        getUnitsSentFrom(send_pattern,task_rank,&snt_cnts[0]);
        size_t offset = 0;
        //buffer_offset<D> bo;
        //bo.buffer = &buffer[0];
        for(unsigned ii = 0; ii < num_snt_frm; ii++)
        {
          int inter_rnk = t1s+snt_rnks[ii]; // hacky and awful
          PCU_Comm_Write(inter_rnk,
                         &buffer[offset],
                         snt_cnts[ii]*tp_sz);
          offset += snt_cnts[ii];
        }
        PCU_Comm_Send();
        int recvfrom = -1;
        void * recv;
        size_t recv_size;
        while(PCU_Comm_Read(&recvfrom,&recv,&recv_size))
        { }
      }
      else if (t2 == tl)// recv'ing
      {
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
      }
      // Switch pcu comms back to task
      PCU_Switch_Comm(tl->comm());
    }
  template <typename PRT_IO, typename DATA_IO>
    void ControlService::Communicate(size_t rdd_id, PRT_IO cnts, DATA_IO bfr, MPI_Datatype tp)
  {
    std::pair<size_t,size_t> r_dd_id = rdd_map[rdd_id];
    std::pair<size_t,size_t> t_ids = comm_man->Relation_GetTasks(r_dd_id.first);
    Task * tl = task_man->getLocalTask();
    int task_rank = tl->localRank();
    Task * t1 = task_man->Task_Get(t_ids.first);
    Task * t2 = task_man->Task_Get(t_ids.second);
    int t1s = taskSize(t1);
    int tp_sz = 0;
    MPI_Type_size(tp,&tp_sz);
    PCU_Switch_Comm(comm_man->CommRelation_GetInterComm(r_dd_id.first));
    PCU_Comm_Begin();
    CommPattern * ptrn = NULL; //t1->getLocalDDValue(r_dd_id.second);
    if(tl == t1)
    {
      unsigned cnt_snt_frm = countRanksSentFrom(ptrn,task_rank);
      std::vector<int> snt_rnks(cnt_snt_frm);
      getRanksSentFrom(ptrn,task_rank,&snt_rnks[0]);
      std::vector<int> snt_cnts(cnt_snt_frm);
      getUnitsSentFrom(ptrn,task_rank,&snt_cnts[0]);
      int unt = 0;
      size_t offset = 0;
      for(unsigned ii = 0; ii < cnt_snt_frm; ii++)     // for each rank recving from this one
      {
        int rnk = t1s+snt_rnks[ii]; //hacky and awful
        size_t cnt = 0;
        size_t * prts = new size_t[snt_cnts[ii]];
        for(int jj = 0; jj < snt_cnts[ii]; jj++)       // for each unit being sent to the foreign rank
        {
          prts[jj] = cnts[unt];
          cnt += cnts[unt];                            // add the number of parts for the unit to the ammount being sent
          unt++;                                       // on to the next unit
        }
        size_t prts_sz = snt_cnts[ii] * sizeof(size_t);
        size_t rnk_bfr_sz = cnt*tp_sz;
        char * rnk_bfr = new char[prts_sz+rnk_bfr_sz];
        memcpy(&prts[0],&rnk_bfr[0],prts_sz);
        memcpy(&bfr[offset],&rnk_bfr[0]+prts_sz,rnk_bfr_sz);
        PCU_Comm_Write(rnk,&rnk_bfr[offset],prts_sz+rnk_bfr_sz);    // send all parts of all units being sent to the foreign rank
        offset += cnt;                                              // update the buffer offset
        delete [] prts;
        delete [] rnk_bfr;
      }
      PCU_Comm_Send();
      int frm = -1;
      void * rcv = NULL;
      size_t rcv_sz = 0;
      while(PCU_Comm_Read(&frm,&rcv,&rcv_sz)) { }
    }
    else
    {
      PCU_Comm_Send();
      size_t bfr_sz = 0;
      size_t bfr_hd = 0;
      size_t prt_hd = 0;
      size_t rcv_sz = 0;
      int frm = -1;
      int lst_frm = -1;
      void * rcv = NULL;
      std::vector<int> cnts(t1s);
      getUnitsSentFrom(ptrn,frm,&cnts[0]);
      while(PCU_Comm_Read(&frm,&rcv,&rcv_sz))
      { // need to make sure order is consistent, order by sending rank
        unsigned rcv_cnt = cnts[frm];
        size_t old_sz = bfr_sz;
        size_t old_cnt_sz = cnts.size();
        bfr_sz += rcv_cnt;
        bfr.resize(bfr.size()+rcv_cnt); // this is stupid and awful
        cnts.resize(cnts.size()+rcv_cnt); // this is also stupid and awful
        size_t prt_sz = rcv_cnt * sizeof(size_t);
        if(frm > lst_frm)
        {
          memcpy(&cnts[prt_hd],rcv,prt_sz);
          memcpy(&bfr[bfr_hd],(void*)((size_t)rcv+prt_sz),rcv_sz-prt_sz);
        }
        else
        {
          // this also is stupid and awful
          memcpy(&cnts[prt_hd],&cnts[0],old_cnt_sz);
          memcpy(&cnts[0],rcv,prt_sz);
          memcpy(&bfr[bfr_hd],&bfr[0],old_sz);
          memcpy(&bfr[0],(void*)((size_t)rcv+prt_sz),rcv_sz-prt_sz);
        }
        bfr_hd += rcv_cnt;
        prt_hd += rcv_cnt;
        lst_frm = frm;
      }
    }
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
        addDataFctn algo = NULL;
        switch(option)
        {
        case -1: // User specified function
          // userFunc
          break;
        default:
          std::cerr << "Warning: AMSI Load Balancing option not defined - proceeding with default option" << std::endl;
        case 0:
          algo = addData_evenSpread;
          break;
        case 1:
          algo = addData_leastFirst;
          break;
        case 2:
          algo = addData_randTest;
          break;
        }
        (*algo)(pattern,delta_pattern,&numNewData[0]);
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
      PCU_Switch_Comm(task_comm);
      PCU_Comm_Begin();
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
            PCU_Comm_Write(m_send_to[current_send],
                           migration_data[current_send].data(),
                           migration_data[current_send].size());
            current_send++;
          }
          current_data++;
        }
      }
      PCU_Comm_Send();
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
      (*dd) += -1 * m_send_to.size();
      migration_data.clear();
      m_index.clear();
      m_send_to.clear();
      // ***** RECEIVE MIGRATION DATA ***** //
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
        (*dd) +=  migration_data.size();
      }
      amsi::Assemble(dd,task_comm);
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
      int recv_from;
      void* recv;
      size_t recv_size;
      PCU_Switch_Comm(comm_man->CommRelation_GetInterComm(r_dd_id.first));
      PCU_Comm_Begin();
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
          PCU_Comm_Write(m_t1process[ii],dataToSend,2*sizeof(int));
        }
        PCU_Comm_Send();
        while(PCU_Comm_Read(&recv_from,&recv,&recv_size))
        { }
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
        PCU_Comm_Send();
        while(PCU_Comm_Read(&recv_from,&recv,&recv_size))
        {
          newT2proc.push_back(recv_from - t1s);
          oldCompIndex.push_back( ((int*)recv)[0] );
          oldT2proc.push_back( ((int*)recv)[1] );
        }
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
      PCU_Switch_Comm(tl->comm());
    }
}
