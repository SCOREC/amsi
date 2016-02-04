#include "Assemblable.h"
namespace amsi
{
  int getReconcileSendCount(int t1s, int t2s, int t1rnk)
  {
    int cnt = 0;
    if(t1s > 0 && t1rnk < t1s && t1rnk >= 0)
    {
      int t2_per_t1 = t2s / t1s;
      int extra_t2 = t2s % t1s;
      cnt = t2_per_t1 + (t1rnk < extra_t2);
    }
    return cnt;
  }
  void getReconcileSendRanks(int t1s, int t2s, int t1rnk, int * rnks)
  {
    if(t1s > 0 && t1rnk < t1s && t1rnk >= 0)
    {
      int cnt = getReconcileSendCount(t1s,t2s,t1rnk);
      int t2_per_t1 = t2s / t1s;
      int extra_t2 = t2s % t1s;
      for(int ii = 0; ii < cnt; ii++)
        rnks[ii] = t1rnk*(t2_per_t1 + (t1rnk <= extra_t2))
          + ((t1rnk > extra_t2) * extra_t2) + ii;
    }
  }
  int getNthReconcileSendRank(int t1s, int t2s, int t1rnk, int nth)
  {
    int rnk = -1;
    if(t1s > 0 && t1rnk < t1s && t1rnk >= 0)
    {
      int t2_per_t1 = t2s / t1s;
      int extra_t2 = t2s % t1s;
      rnk = t1rnk*(t2_per_t1 + t1rnk <= extra_t2)
          + (t1rnk > extra_t2) * extra_t2 + nth;
    }
    return rnk;
  }
}
