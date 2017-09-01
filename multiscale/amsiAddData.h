#ifndef AMSI_ADDDATA_LOADBALANCING_H_
#define AMSI_ADDDATA_LOADBALANCING_H_
#include "amsiDataDistribution.h"
#include <cassert>
#include <iterator>
#include <vector>
namespace amsi
{
  class CommPattern;
  typedef void (*addDataFctn)(CommPattern*, CommPattern*, DataDistribution*);
  class DynamicAddAlgorithms
  {
  private:
    std::vector<addDataFctn> algos;
  public:
    int addAlgo(addDataFctn algo)
    {
      algos.push_back(algo);
      return std::distance(algos.begin(),algos.end());
    }
    addDataFctn getAlgo(int idx)
    {
      assert((unsigned) idx < algos.size());
      return algos[idx];
    }
  };
  void addData_evenSpread(CommPattern * cp, CommPattern * dlta, DataDistribution * dd_dlta);
  void addData_leastFirst(CommPattern * cp, CommPattern * dlta, DataDistribution * dd_dlta);
  void addData_randTest(CommPattern * cp, CommPattern * dlta, DataDistribution * dd_dlta);
} // namespace amsi
#endif
