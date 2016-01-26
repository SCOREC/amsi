#include "amsiScale.h"
#include "amsiRankSet.h"
namespace amsi
{
  void Scale::assignRanks(RankSet * rs, MPI_Comm cm)
  {
    RankSet * ns = new DefaultRankSet;
    merge(rnks,rs,ns);
    delete rnks;
    rnks = ns;
    setValid(false);
  }
  bool Scale::validResign(RankSet * rs, MPI_Comm cm)
  {
    bool result = false;
    DefaultRankSet intrsctn;
    intersection(rnks,rs,&intrsctn);
    if(intrsctn.size() == rs->size())
      result = true;
    return true;
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
    const RankSet * rs = scl->getRankSet();
    if(rs->contains(rnk))
      result = true;
    return result;
  }
  void assignFirstN(const RankSet * rs, rank_t n, Scale * sc)
  {
    DefaultRankSet a, b;
    takeN(rs,n,&a,&b);
    sc->assignRanks(&a);
  }
}
