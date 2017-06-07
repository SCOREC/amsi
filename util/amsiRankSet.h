#ifndef AMSI_RANK_SET_H_
#define AMSI_RANK_SET_H_
#include "amsiMPI.h"
#include <set>
namespace amsi
{
  class RankSet
  {
  public:
    virtual ~RankSet() {};
    virtual int size() const = 0;
    virtual void insert(int) = 0;
    virtual bool contains(int) const = 0;
    virtual void toArray(int *) const = 0;
    virtual RankSet * copy() = 0;
  };
  class SimpleRankSet : public RankSet
  {
  protected:
    std::set<int> s;
  public:
    virtual int size() const {return s.size();}
    virtual void insert(int r) {s.insert(r);}
    virtual bool contains(int r) const {return s.count(r) > 0;}
    virtual void toArray(int * rs) const;
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
  /// todo: these need to be rewritten to avoid use of toArray, which might mean moving them into the class or something similar, but serializing the rank set will scale very poorly
  void intersection(const RankSet * a, const RankSet * b, RankSet * rslt);
  void merge(const RankSet * a, const RankSet * b, RankSet * rslt);
  void diff(const RankSet * a, const RankSet * b, RankSet * rslt);
  void takeN(const RankSet * src, int nm, RankSet * a, RankSet * b);
  enum RankFormats
  {
    SET,
    RANGE,
    FNCTN
  };
  class RankRange
  {
  protected:
    int lw;
    int hgh;
  public:
    int size() {return hgh - lw;}
    void insert(int vl)
    {
      if(vl == hgh+1)
        hgh = vl;
      else if (vl == lw-1)
        lw = vl;
      //else
        // split into a tree with a range branch and a set branch
    }
    bool contains(int vl)
    {
      return (vl > lw && vl <= hgh);
    }
  };
}
#include "amsiRankSet_impl.h"
#endif
