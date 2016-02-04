#include "test.h"
#include "Assemblable.h"
int main(int argc, char ** argv)
{
  int failed = 0;
  int t1s = 3;
  int t2s = 5;
  int cnt0 = amsi::getReconcileSendCount(t1s,t2s,0);
  int rnks0[cnt0];
  amsi::getReconcileSendRanks(t1s,t2s,0,&rnks0[0]);
  failed += test("rank zero count",2,cnt0);
  failed += test("rank zero plan 0",0,rnks0[0]);
  failed += test("rank zero plan 1",1,rnks0[1]);
  int cnt1 = amsi::getReconcileSendCount(t1s,t2s,1);
  int rnks1[cnt1];
  amsi::getReconcileSendRanks(t1s,t2s,1,&rnks1[0]);
  failed += test("rank one count",2,cnt1);
  failed += test("rank one plan 0",2,rnks1[0]);
  failed += test("rank one plan 1",3,rnks1[1]);
  int cnt2 = amsi::getReconcileSendCount(t1s,t2s,2);
  int rnks2[cnt2];
  amsi::getReconcileSendRanks(t1s,t2s,2,&rnks2[0]);
  failed += test("rank two count",1,cnt2);
  failed += test("rank two plan 0",4,rnks2[0]);
  int cnt3 = amsi::getReconcileSendCount(t1s,t2s,3);
  failed += test("rank three count",0,cnt3);
  return failed;
}
