#ifndef ADDDATA_LOADBALANCING_H_
#define ADDDATA_LOADBALANCING_H_
#include <algorithm>
namespace amsi
{
  class CommPattern;
  // Just a utility thing for the load balancing algorithm
  typedef std::pair<int,int> data_index;
  bool data_index_compare_greater(const data_index& i,const data_index& j); 
  bool data_index_compare_less(const data_index& i,const data_index& j); 
  void CommPattern_LoadBalance_Spread(CommPattern * pattern, CommPattern * init, int * num);
  void CommPattern_LoadBalance_LeastFirst(CommPattern * pattern, CommPattern * init, int * num);
  void CommPattern_LoadBalance_Test(CommPattern * pattern, CommPattern * init, int * num,int rank);
} // namespace amsi
#endif
