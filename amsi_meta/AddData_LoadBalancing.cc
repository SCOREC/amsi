#include "AddData_LoadBalancing.h"

#include <cassert>
#include <iostream>
#include <limits.h>
#include <cstdlib>

namespace amsi {

    // Utility for load balancing function (pair sort)
  bool data_index_compare_greater(const data_index& i,const data_index& j) 
  { return i.first > j.first; }

  bool data_index_compare_less(const data_index& i,const data_index& j) 
  { return i.first < j.first; }


  // Load balancing with added data (no migration)
  // Updates pattern (current comm pattern) and init (initialization comm pattern) 
  //   by distributing the data counts specified in num
  // This load balancing uses the default algorithm in CommPattern_CreateDistro to
  //   essentially spew new data accross the comm pattern without regard for filling
  //   lighter loaded processes. So essentially it only load balances the new data.
  // This algorithm is efficient but doesn't really load balance
  void CommPattern_LoadBalance_Spread(CommPattern * pattern, CommPattern * init, int * num)
  {

    int s1 = pattern->getSendTaskSize();
    int s2 = pattern->getRecvTaskSize();
    int total_data = 0;
    for(int ii = 0; ii < s1; ii++)
    {
      total_data += num[ii];
    }
    int even_distro = total_data / s2;
    int extra_distro = total_data % s2;
    int dst = 0;
    int dst_rcv = even_distro + (dst < extra_distro);

    for(int src = 0; src < s1; src++)
    {
      int src_count = num[src];
      while(src_count > 0)
      {
	// if there are enough to send all needed to dstCommPattern_LoadBalance_Spread
	if(src_count >= dst_rcv)
	{
	  (*pattern)(src,dst) += dst_rcv;
	  (*init)(src,dst) = dst_rcv;
	  src_count -= dst_rcv;
	  dst++;
	  dst_rcv = even_distro + (dst < extra_distro);
	}
	else if(src_count < dst_rcv)
	{
	  (*pattern)(src,dst) += src_count;
	  (*init)(src,dst) = src_count;
	  dst_rcv -= src_count;
	  src_count = 0;
	}
      }
    }

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
  void CommPattern_LoadBalance_LeastFirst(CommPattern * pattern, CommPattern * init, int * num)
  {

    int s1 = pattern->getSendTaskSize();
    int s2 = pattern->getRecvTaskSize();
    int tempsum;
    int sums[s2];

    // Sum number of data on each recv process, and find max
    int maxsum = INT_MIN;
    int maxjj = 0;
    for(int jj=0;jj<s2;jj++)
    {
      tempsum = 0;
      for(int kk=0;kk<s1;kk++)
	tempsum += (*pattern)(kk,jj);
      sums[jj] = tempsum; // Store sum
      if(tempsum > maxsum)
      {
	maxsum = tempsum;
	maxjj = jj;
      }
    }

    // Compute difference between max sum and other recv process's sums
    std::vector<data_index> deltas;
    deltas.resize(s2);
    for(int jj=0;jj<s2;jj++)
    {
      deltas[jj].first  = maxsum - sums[jj];
      deltas[jj].second = jj;
    }

    // Sort the deltas (while remembering index)
    std::sort(deltas.begin(),deltas.end(),data_index_compare_greater);

    // Fill pattern based on deltas
    // while ( there is still extra data and the loads aren't all even )
    //   add one data to all lightest loaded processes
    bool done = false;
    int jj = 0;
    int ii = 0;

    if (deltas[0].first==0) done = true; // All loads are even
    while(ii<s1 && num[ii]==0)
      ii++;
    if (ii >= s1) done = true; // No data to add

    while(!done){
      int curDelta = deltas[0].first;
      bool stepDone = false;
      jj = 0;
      while(!stepDone)
      {
	// Update patterns and data counts
	(*pattern)(ii,deltas[jj].second) ++;
	(*init)(ii,deltas[jj].second) ++;
	num[ii]--;
	deltas[jj].first--;

	// Check if there is still data to add
	while(ii<s1 && num[ii]==0)
	  ii++;
	if(ii >= s1)
	{
	  stepDone = true;
	  done = true;
	}

	// All lightest loads have had one added to them
	if(deltas[jj+1].first != curDelta)
	  stepDone = true;

	jj++;

      }

      if (deltas[0].first==0) done=true;  // All loads are even

    }

    // Distribute any remaining data by default algorithm
    int total_data = 0;
    for(ii = 0; ii < s1; ii++)
    {
      total_data += num[ii];
    }

    if (total_data == 0) return;

    int even_distro = total_data / s2;
    int extra_distro = total_data % s2;
    int dst = 0;
    int dst_rcv = even_distro + (dst < extra_distro);

    for(int src = 0; src < s1; src++)
    {
      int src_count = num[src];
      while(src_count > 0)
      {
	// if there are enough to send all needed to dst
	if(src_count >= dst_rcv)
	{
	  (*pattern)(src,dst) += dst_rcv;
	  (*init)(src,dst) += dst_rcv;
	  src_count -= dst_rcv;
	  dst++;
	  dst_rcv = even_distro + (dst < extra_distro);
	}
	else if(src_count < dst_rcv)
	{
	  (*pattern)(src,dst) += src_count;
	  (*init)(src,dst) += src_count;
	  dst_rcv -= src_count;
	  src_count = 0;
	}
      }
    }

  }

  void CommPattern_LoadBalance_Test(CommPattern * pattern, CommPattern * init, int * num,int rank)
  {
    assert(pattern);
    assert(init);
    assert(num);

    int s2 = pattern->getRecvTaskSize();
    srand(time(NULL));
    for(int ii=0;ii<num[rank];ii++)
    {
      int addTo = rand()%s2;
      (*pattern)(rank,addTo)++;
      (*init)(rank,addTo)++;
    }
  }
} // namespace amsi
