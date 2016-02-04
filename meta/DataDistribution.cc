#include "DataDistribution.h"
#include "amsiMPI.h"
#ifdef ZOLTAN
#include <zoltan.h>
#endif
namespace amsi
{
  DataDistribution::DataDistribution(int size)
    : valid(false)
    , wgts(size)
    , dd(size)
  {  }
# ifdef ZOLTAN
  DataDistribution::DataDistribution(int size, Zoltan_Struct * z)
    : valid(false)
    , wgts(size)
    , dd(size)
    , zs(z)
  { }
#endif
  void DataDistribution::Assemble(MPI_Comm comm)
  {
    int rnk = 0;
    MPI_Comm_rank(comm,&rnk);
    int lcl = dd[rnk];
    MPI_Allgather(&lcl,1,mpi_type<int>(),
                  &dd[0],1,mpi_type<int>(),
                  comm);
    for(unsigned ii = 0; ii < dd.size(); ii++)
      wgts[ii].resize(dd[ii]);
    assembled = true;
  }
  int DataDistribution::operator [](unsigned index) const
  {
    return dd[index];
  }
  int& DataDistribution::operator [](unsigned index)
  {
    unassembled();
    return dd[index];
  }
  void DataDistribution::setWeight(unsigned index, unsigned sub_index, double value)
  {
    if(!assembled)
    {
      // HACKY really should assemble here... but we're unlikely to hit this case
      wgts[index].resize(dd[index]);
      valid = true;
    }
    wgts[index][sub_index] = value;
  }
  double DataDistribution::getWeight(unsigned index, unsigned sub_index)
  {
    if(sub_index+1 > wgts[index].size())
      wgts[index].resize(sub_index+1,0.0);
    return wgts[index][sub_index];
  }
} // namespace amsi
