#include "amsiDataDistribution.h"
#include "amsiMPI.h"
#ifdef ZOLTAN
#include <zoltan.h>
#endif
namespace amsi
{
  DistributedData::DistributedData(Scale * s, int cnt)
    : Distributed(s->getComm())
    , scl(s)
    , lcl_cnt(cnt)
    , wgts(lcl_cnt)
  { }
  void DistributedData::synchronize()
  {
    wgts.resize(lcl_cnt);
    setValid(true);
  }
  int DistributedData::operator[](unsigned idx) const
  {
    int bfr = 0;
    MPI_Bcast(&bfr,1,MPI_INTEGER,idx,scl->getComm());
    return bfr;
  }
  int & DistributedData::operator[](unsigned idx)
  {
    int rnk = -1;
    MPI_Comm_rank(scl->getComm(),&rnk);
    assert(idx == static_cast<unsigned>(rnk));
    setValid(false);
    return lcl_cnt;
  }
  void DistributedData::setWeight(unsigned idx, double vl)
  {
    assert(idx < wgts.size());
    wgts[idx] = vl;
  }
  double DistributedData::getWeight(unsigned idx)
  {
    assert(idx < wgts.size());
    return wgts[idx];
  }
# ifdef ZOLTAN
  DataDistribution::DataDistribution(int s, int l, bool w, Zoltan_Struct * z)
    : sz(s)
    , lcl_rnk(l)
    , valid(false)
    , wgtd(w)
    , wgts(wgtd ? sz : 0)
    , dd(sz)
    , zs(z)
  { }
#else
  DataDistribution::DataDistribution(int s, int l, bool w)
    : sz(s)
    , lcl_rnk(l)
    , valid(false)
    , wgtd(w)
    , wgts(wgtd ? sz : 0)
    , dd(sz)
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
    if(wgtd)
      for(unsigned ii = 0; ii < dd.size(); ii++)
        wgts[ii].resize(dd[ii]);
    assembled = true;
  }
  int DataDistribution::operator[](unsigned idx) const
  {
    if(sz)
      return dd[idx];
    return -1;
  }
  int DataDistribution::operator=(int qnt)
  {
    if(sz)
    {
      unassembled();
      return dd[lcl_rnk] = qnt;
    }
    return -1;
  }
  int DataDistribution::operator+=(int qnt)
  {
    if(sz)
    {
      unassembled();
      return dd[lcl_rnk] += qnt;
    }
    return -1;
  }
  double & DataDistribution::getWeight(unsigned nth, unsigned wgt)
  {
    if(!assembled)
    {
      // HACKY really should assemble here... but we're unlikely to hit this case
      wgts[nth].resize(dd[nth]);
      valid = true;
    }
    return wgts[nth][wgt];
  }
} // namespace amsi
