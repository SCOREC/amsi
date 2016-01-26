#include "amsiPermute.h"
namespace amsi
{
  void combinePermutations(int * p1, int * p2, int sz)
  {
    for(int ii = 0; ii < sz; ii++)
      p1[ii] = p2[p1[ii]];
  }
}
