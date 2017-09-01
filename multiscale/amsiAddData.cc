#include "amsiAddData.h"
#include "amsiCoupling.h"
#include "amsiPermute.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits.h>
#include <cstdlib>
namespace amsi
{
  // Load balancing with added data (no migration)
  // Updates pattern (current comm pattern) and init (initialization comm pattern)
  //   by distributing the data counts specified in num
  // This load balancing uses the default algorithm in CommPattern_CreateDistro to
  //   essentially spew new data accross the comm pattern without regard for filling
  //   lighter loaded processes. So essentially it only load balances the new data.
  // This algorithm is efficient but doesn't really load balance
  void addData_evenSpread(CommPattern * cp, CommPattern * dlta, DataDistribution * dd_dlta)
  {
    int s1 = cp->getNumSenders();
    int s2 = cp->getNumRecvers();
    int dd[s1]{};
    for(int ii = 0; ii < s1; ++ii)
      dd[ii] = (*dd_dlta)[ii];
    int total_data = 0;
    for(int ii = 0; ii < s1; ii++)
      total_data += (*dd_dlta)[ii];
    int even_distro = total_data / s2;
    int extra_distro = total_data % s2;
    int dst = 0;
    int dst_rcv = even_distro + (dst < extra_distro);
    for(int src = 0; src < s1; src++)
    {
      int src_count = (*dd_dlta)[src];
      while(src_count > 0)
      {
        // if there are enough to send all needed to dstCommPattern_LoadBalance_Spread
        if(src_count >= dst_rcv)
        {
          (*cp)(src,dst) += dst_rcv;
          (*dlta)(src,dst) = dst_rcv;
          src_count -= dst_rcv;
          dst++;
          dst_rcv = even_distro + (dst < extra_distro);
        }
        else if(src_count < dst_rcv)
        {
          (*cp)(src,dst) += src_count;
          (*dlta)(src,dst) = src_count;
          dst_rcv -= src_count;
          src_count = 0;
        }
      }
    }
    int rnk = -1;
    MPI_Comm_rank(AMSI_COMM_SCALE,&rnk);
    (*dd_dlta) = dd[rnk];
  }
  // Load balancing with added data (no migration)
  // Updates pattern (current comm pattern) and init (initialization comm pattern)
  //   by distributing the data counts specified in num
  // This load balancing algorithm finds the lightest loaded processes and
  //   adds data to them first. This is the opposite end of the spectrum from
  //   CommPattern_LoadBalance_Spread. i.e. this does perfect-ish load
  //   balancing but is inefficient.
  // Any left over data, after filling lighter loads is sorted by using the
  //   algorithm found in both CommPattern_CreateDistro and CommPattern_LoadBalance_Spread
  // i hate the way this is written and it needs to be improved / made more clear
  void addData_leastFirst(CommPattern * cp, CommPattern * init, DataDistribution * dd_dlta)
  {
    int s1 = cp->getNumSenders();
    int s2 = cp->getNumRecvers();
    int dd[s1]{};
    for(int ii = 0; ii < s1; ++ii)
      dd[ii] = (*dd_dlta)[ii];
    int cnts[s2];
    getUnitsRecv(cp,cnts);
    int mx = 0;
    for(int ii = 0; ii < s2; ii++)
      mx = cnts[ii] > mx ? cnts[ii] : mx;
    // Compute difference between max sum and other recv process's sums
    unsigned prm[s2];
    int dlts[s2];
    for(int jj = 0; jj < s2; jj++)
    {
      dlts[jj] = mx - cnts[jj];
      prm[jj] = jj;
    }
    // todo: TEST THIS
//    std::sort(&prm[0],&prm[s2-1],permuteComp(&dlts[0],std::greater<int>()));
    // Fill pattern based on deltas
    // while ( there is still extra data and the loads aren't all even )
    //   add one data to all lightest loaded processes
    bool even = false;
    bool all_assgnd = false;
    // check if data is already evenly distributed, default to even spread
    if(dlts[prm[0]] == 0)
      even = true; // All loads are even
    int idx = 0;
    // get first sending index with data to add
    while(idx < s1 && dd[idx] == 0)
      idx++;
    // if there was nothing to add, then everything has been assigned
    if(idx >= s1)
      all_assgnd = true;
    while(!even && !all_assgnd)
    {
      int add_cnt = dlts[prm[0]];
      int jdx = 0;
      bool assgnd = false;
      while(!assgnd)
      {
        // Update patterns and data counts
        (*cp)(idx,prm[jdx])++;
        (*init)(idx,prm[jdx])++;
        dd[idx]--;
        dlts[prm[jdx]]--;
        // Check if there is still data to add
        while(idx < s1 && dd[idx] == 0)
          idx++;
        if(idx >= s1)
        {
          all_assgnd = true;
          assgnd = true;
        }
        // All lightest loads have had one added to them
        if(dlts[prm[jdx+1]] != add_cnt)
          assgnd = true;
        jdx++;
      }
      // Check if loads are now even
      if (dlts[prm[0]]==0)
        even = true;
    }
    // Distribute any remaining data by default algorithm
    int total_data = 0;
    for(int ii = 0; ii < s1; ii++)
      total_data += dd[ii];
    if (total_data == 0)
      return;
    int even_distro = total_data / s2;
    int extra_distro = total_data % s2;
    int dst = 0;
    int dst_rcv = even_distro + (dst < extra_distro);
    for(int src = 0; src < s1; src++)
    {
      int src_count = dd[src];
      while(src_count > 0)
      {
        // if there are enough to send all needed to dst
        if(src_count >= dst_rcv)
        {
          (*cp)(src,dst) += dst_rcv;
          (*init)(src,dst) += dst_rcv;
          src_count -= dst_rcv;
          dst++;
          dst_rcv = even_distro + (dst < extra_distro);
        }
        else if(src_count < dst_rcv)
        {
          (*cp)(src,dst) += src_count;
          (*init)(src,dst) += src_count;
          dst_rcv -= src_count;
          src_count = 0;
        }
      }
    }
    int rnk = -1;
    MPI_Comm_rank(AMSI_COMM_SCALE,&rnk);
    (*dd_dlta) = dd[rnk];
  }
  void addData_randTest(CommPattern * cp, CommPattern * init, DataDistribution * num)
  {
    assert(cp);
    assert(init);
    assert(num);
    // assumes that that sending task of cp is related to local comm, which is true for the static process-set assignment case but would not be generally...
    int rnk = -1;
    MPI_Comm_rank(AMSI_COMM_SCALE,&rnk);
    int s2 = cp->getNumRecvers();
    srand(8675309);
    for(int ii = 0; ii < (*num)[rnk]; ii++)
    {
      int addTo = rand()%s2;
      (*cp)(rnk,addTo)++;
      (*init)(rnk,addTo)++;
    }
  }
} // namespace amsi
