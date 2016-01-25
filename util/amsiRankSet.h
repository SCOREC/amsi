#ifndef AMSI_RANK_SET_H_
#define AMSI_RANK_SET_H_
#include "amsiMPI.h"
#include <set>
namespace amsi
{
  class RankSet
  {
  public:
    virtual rank_t size() const = 0;
    virtual void insert(rank_t) = 0;
    virtual bool contains(rank_t) const = 0;
    virtual void toArray(rank_t *) const = 0;
    virtual RankSet * copy() = 0;
  };
  class SimpleRankSet : public RankSet
  {
  protected:
    std::set<rank_t> s;
  public:
    virtual rank_t size() const {return s.size();}
    virtual void insert(rank_t r) {s.insert(r);}
    virtual bool contains(rank_t r) const {return s.count(r) > 0;}
    virtual void toArray(rank_t * rs) const;
    virtual RankSet * copy();
  };
  typedef SimpleRankSet DefaultRankSet;
  /// note: container must have operator[] member function
  template <typename T>
    void insertFromContainer(RankSet * rs, T & rnks);
  template <typename T>
    void pushToContainer(RankSet * rs, T begin, T end);
  /// note: collective on the supplied communicator
  void getGlobalRanks(RankSet*,MPI_Comm = AMSI_COMM_WORLD);
  /// note: collective on the RankSet if using MPI3 standard
  MPI_Comm makeComm(RankSet*);
  void intersection(const RankSet * a, const RankSet * b, RankSet * rslt);
  void merge(const RankSet * a, const RankSet * b, RankSet * rslt);
  void diff(const RankSet * a, const RankSet * b, RankSet * rslt);
}
#endif
#include "amsiRankSet_impl.h"
