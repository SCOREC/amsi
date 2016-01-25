#ifndef AMSI_PERMUTE_H_
#define AMSI_PERMUTE_H_
#include <algorithm>
#include <vector>
namespace amsi
{
  template <typename T, typename OP>
    struct PermuteComp
  {
  private:
    T frst;
    OP op;
  public:
    PermuteComp(T f, OP p)
      : frst(f)
      , op(p)
    {}
    bool operator()(unsigned a, unsigned b)
    {
      return op((*frst+a),(*frst+b));
    }
  };
  template <typename T, typename OP>
    PermuteComp<T,OP> permuteComp(T f, OP p)
  {
    return PermuteComp<T,OP>(f,p);
  }
  void combinePermutations(int * p1, int * p2, int sz);
}
#endif
