#include "test.h"
#include "amsiMeta.h"
#include <iostream>
#include <utility>
using namespace amsi;
// should not be called
int task0(int&,char**&,MPI_Comm)
{
  return 1;
}
// should be called
int task1(int&,char**&,MPI_Comm)
{
  return 0;
}
int main(int argc, char * argv[])
{
  initializer = new amsi::amsiMetaInit();
  amsiInit(argc,argv);
  int rank = -1;
  MPI_Comm_rank(AMSI_COMM_SCALE,&rank);
  int size = 0;
  MPI_Comm_size(AMSI_COMM_SCALE,&size);
  int failed = 0;
  // allocated zero processes to task
  std::cout << "Creating ProcessSet with 0 processes" << std::endl;
  ProcessSet * ps0 = static_cast<ProcessSet*>(new ProcessSet_T<std::pair<int, int> >(-1,-1));
  Task * t0 = new Task(ps0);
  failed += test_neq("Task(0)",static_cast<Task*>(NULL),t0);
  t0->createDD("t0_data");
  failed += test(".verifyDD()",false,t0->verifyDD("t0_data"));
  size_t dd_id0 = t0->getDD_ID("t0_data");
  failed += test_neq(".getDD_ID()",(size_t)0,dd_id0);
  failed += test(".localRank()",-1,t0->localRank());
  failed += test(".localToGlobalRank(-1)",-1,t0->localToGlobalRank(-1));
  failed += test(".localToGlobalRank(0)",-1,t0->localToGlobalRank(0));
  t0->setExecutionFunction(&task0);
  failed += test(".execute()",0,t0->execute(argc,argv));
  t0->setLocalDDValue("t0_data",rank % 3);
  failed += test(".getLocalDDValue()",-1,t0->getLocalDDValue(dd_id0));
  //t0->DataDist_Assemble("t0_data");
  // task manager allocates some free processes
  std::cout << "Creating ProcessSet with 0,size where size = "<<size<<std::endl;
  ProcessSet * ps = static_cast<ProcessSet*>(new ProcessSet_T<std::pair<int, int> >(0,size));
  Task * t1 = new Task(ps);
  failed += test_neq("Task()",static_cast<Task*>(NULL),t1);
  t1->createDD("t1_data");
  failed += test(".verifyDD(\"t1_data\")",true,t1->verifyDD("t1_data"));
  failed += test(".verifyDD(\"t1_data_fail\")",false,t1->verifyDD("t1_data_fail"));
  size_t dd_id = t1->getDD_ID("t1_data");
  failed += test_neq(".getDD_ID()",static_cast<size_t>(0),dd_id);
  failed += test(".localRank()",rank,t1->localRank());
  failed += test(".localToGlobalRank()",size >= 4 ? 4 : -1,t1->localToGlobalRank(4));
  t1->setExecutionFunction(&task1);
  failed += test(".execute()",0,t1->execute(argc,argv));
  t1->setLocalDDValue("t1_data",rank % 3);
  failed += test(".getLocalDDValue()",rank % 3,t1->getLocalDDValue(dd_id));
  //t1->DataDist_Assemble("t1_data");
  amsiFree();
  return failed;
}
