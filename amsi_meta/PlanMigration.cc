#include "PlanMigration.h"
#include "DataDistribution.h"
#include "amsiCoupling.h"
#ifdef ZOLTAN
#include <zoltan.h>
#endif
#include <iostream>
#include <limits.h>
#include <cstdlib>
namespace amsi
{
    // For now this is the default migration planning routine
    // TODO: get rid of comm pattern, use data distribution
    // General description:
    //  - Fills the m_send_to vector with the process number to send to
    //    and the m_index vetor with the local index of each piece of
    //    data to be sent
    //  - These vectors must be sorted by local index
    //  - This function does not update the comm pattern
    //
    // Specific description:
    //  - This plan is meant to test migration routines, not
    //    to be a good plan
    //  - Treats all data equally
    //  - Calculates average workload accross task
    //  - Calculates threshold based on average workload
    //  - Adds data to send list until below threshold
    //  - Sends to random processes
    void CommPattern_PlanMigration_Test(std::vector<int> & m_index,
                                        std::vector<int> & m_send_to,
                                        CommPattern * pattern,
                                        const DataDistribution * dd,
                                        int rank,
                                        MPI_Comm task_comm)
    {
      int s1 = pattern->getNumSenders();
      int s2 = pattern->getNumRecvers();

      srand(time(NULL) + rank);

      // Sum local workload
      int local_total = 0;
      for(int ii=0;ii<s1;ii++)
        local_total += (*pattern)(ii,rank);

      // Sum total workload
      int total = 0;
      int allTotals[s2];
      MPI_Allreduce(&local_total,&total,1,MPI_INT,MPI_SUM,task_comm);
      MPI_Allgather(&local_total,1,MPI_INT,&allTotals,1,MPI_INT,task_comm);

      // Calculate average workload per process
      double avgWork = total/(double)s2;

      // Compare local workload to average
      double percent = 0.02;

      int kk = 0;
      int ll = 0;
      bool sendNow;
      if(local_total > avgWork + 1)
      {
        // Add to send list until local total is closer to average
        while(local_total > avgWork*(1.0+percent))
        {
          // Send to random process in task
          int send_to = rand() % s2;
          sendNow = false;

          if(allTotals[send_to] < avgWork && send_to != rank)
          {
            sendNow = true;
          }
          else
          {
            ll++;
            if(ll > s2  && send_to != rank)
              sendNow = true;
          }

          if(sendNow)
          {
            m_index.push_back(kk);
            m_send_to.push_back(send_to);

            kk++;
            local_total--;
          }
        }
      }

    }



    void CommPattern_PlanMigration_Full(std::vector<int> & m_index,
                                        std::vector<int> & m_send_to,
                                        CommPattern * pattern,
                                        const DataDistribution * dd,
                                        int rank,
                                        MPI_Comm task_comm)
    {
      int s1 = pattern->getNumSenders();
      int s2 = pattern->getNumRecvers();

      // Sum local workload
      int local_total = 0;
      for(int ii=0;ii<s1;ii++)
        local_total += (*pattern)(ii,rank);

      // Sum total workload
      int allTotals[s2];
      MPI_Allgather(&local_total,1,MPI_INT,&allTotals,1,MPI_INT,task_comm);

      int total = 0;
      for(int ii = 0; ii < s2; ii++)
        total += allTotals[ii];

      int d1 = total/s2;
      int d2 = total%s2;

      std::vector<data_index> totals;
      totals.resize(s2);
      for(int jj=0;jj<s2;jj++)
      {
        totals[jj].first  = allTotals[jj];
        totals[jj].second = jj;
      }

      std::sort(totals.begin(),totals.end(),data_index_compare_less);

      int current_send_to = 0;
      int ii=s2-1;
      while( totals[ii].first > (d1 + (ii >= (s2-d2))) )
      {
        int kk = 0;
        int amount_over = totals[ii].first - (d1 + (ii >= (s2-d2)));
        for(int jj=0;jj<amount_over;jj++)
        {
          // if the process to which we're planning to send data is full
          bool current_full = totals[current_send_to].first >= ( d1 + (current_send_to >= (s2-d2)) );
          if(current_full)
          {
            current_send_to++;
            jj--;
          }
          else
          {
            // remove one from the current sending process
            totals[ii].first--;
            // add one to the current recving process
            totals[current_send_to].first++;

            if(rank == totals[ii].second)
            {
              // local datadist index of data to send
              m_index.push_back(kk++);

              // process to which the data is being sent
              m_send_to.push_back(current_send_to);
            }
          }
        }
        // move to the process with the next largest weights
        ii--;
      }
    }

# ifdef ZOLTAN
  void CommPattern_PlanMigration_Zoltan(std::vector<int> & m_index,
                                        std::vector<int> & m_send_to,
                                        CommPattern * pattern,
                                        const DataDistribution * dd,
                                        int rank,
                                        MPI_Comm task_comm)
  {
    Zoltan_Struct * zs = dd->GetZS();

    // get the zoltan struct ready
    Zoltan_Set_Param(zs,"RETURN_LISTS","EXPORT"); // only generate sending lists
    Zoltan_Set_Param(zs,"OBJ_WEIGHT_DIM","1"); //
    Zoltan_Set_Param(zs,"LB_METHOD","BLOCK");

    int do_lb = 0;
    int num_gid_entries = 0;
    int num_lid_entries = 0;
    int num_recv = 0;
    int num_send = 0;

    ZOLTAN_ID_TYPE * recv_gids; // global object IDs to recv
    ZOLTAN_ID_TYPE * recv_lids; // "1 part per process" so this is not used
    ZOLTAN_ID_TYPE * send_gids; // global object IDS to send
    ZOLTAN_ID_TYPE * send_lids; // "1 part per process" so this is not used

    int * recv_ranks; // ranks from which to recv each object
    int * send_ranks; // ranks to which to send objects

    Zoltan_LB_Balance(zs,
                      &do_lb,
                      &num_gid_entries,
                      &num_lid_entries,
                      &num_recv,
                      &recv_gids,
                      &recv_lids,
                      &recv_ranks,
                      &num_send,
                      &send_gids,
                      &send_lids,
                      &send_ranks);

    if(do_lb)
    {
      m_index.assign(send_lids,send_lids+num_send);
      m_send_to.assign(send_ranks,send_ranks+num_send);
    }

    Zoltan_LB_Free_Data(&recv_gids,
                        &recv_lids,
                        &recv_ranks,
                        &send_gids,
                        &send_lids,
                        &send_ranks);

  }
# endif

} // namespace amsi
