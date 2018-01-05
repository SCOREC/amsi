#include "amsiScale.h"
#include "amsiRankSet.h"
#include <cassert>
#include <numeric>
#include <vector>
namespace amsi
{
  void Scale::assignRanks(RankSet * rs, MPI_Comm )
  {
    RankSet * ns = new DefaultRankSet;
    merge(rnks,rs,ns);
    delete rnks;
    rnks = ns;
    setValid(false);
  }
  bool Scale::validResign(RankSet * rs, MPI_Comm )
  {
    bool result = false;
    DefaultRankSet intrsctn;
    intersection(rnks,rs,&intrsctn);
    if(intrsctn.size() == rs->size())
      result = true;
    return result;
  }
  void Scale::resignRanks(RankSet * rs, MPI_Comm cm)
  {
    if(validResign(rs,cm))
    {
      RankSet * ns = new DefaultRankSet;
      diff(rnks,rs,ns);
      delete rnks;
      rnks = ns;
      setValid(false);
    }
    else
      std::cerr << "ERROR: invalid resignRanks attempt!" << std::endl;
  }
  bool assignedTo(Scale * scl)
  {
    bool result = false;
    int rnk = -1;
    MPI_Comm_rank(AMSI_COMM_WORLD,&rnk);
    if(!scl->isValid())
      std::cout << "Warning: checking process assignment to invalid scale, "
                << "synchronize scale to avoid potential errors." << std::endl;
    const RankSet * rs = scl->getRankSet();
    if(rs->contains(rnk))
      result = true;
    return result;
  }
  void assignFirstN(const RankSet * rs, int n, Scale * sc)
  {
    DefaultRankSet a, b;
    takeN(rs,n,&a,&b);
    sc->assignRanks(&a);
  }
  void assignCounts(const RankSet * rs, int nm, int * cnts, Scale * scls[])
  {
    std::vector<DefaultRankSet> rmndr(nm);
    std::vector<DefaultRankSet> sbsts(nm);
    takeN(rs,cnts[0],&sbsts[0],&rmndr[0]);
    scls[0]->assignRanks(&sbsts[0]);
    for(int ii = 1; ii < nm; ii++)
    {
      takeN(&rmndr[ii-1],cnts[ii],&sbsts[ii],&rmndr[ii]);
      scls[ii]->assignRanks(&sbsts[ii]);
    }
  }
  void assignRatios(const RankSet * rs, int cnt, double * rts, Scale * scls[])
  {
    int sz = rs->size();
    int cnts[cnt];
    for(int ii = 0; ii < cnt; ii++)
      cnts[ii] = (int)(((double)sz)*rts[ii]);
    assert(std::accumulate(&cnts[0],&cnts[cnt-1],0) == sz);
    assignCounts(rs,cnt,&cnts[0],scls);
  }
}
