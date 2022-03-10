#include "test.h"
#include "amsiMultiscale.h"
#include "amsiProcessSet.h"
#include "amsiTask.h"
#include "amsiDataDistribution.h"
#include "amsiMPI.h"
#include <iostream>
#include <utility>
using namespace amsi;
// should not be called
int task0(int&, char**&, MPI_Comm, amsi::Multiscale&) { return 1; }
// should be called
int task1(int&, char**&, MPI_Comm, amsi::Multiscale&) { return 0; }
int main(int argc, char* argv[])
{
  amsi::MPI mpi(argc, argv, MPI_COMM_WORLD);
  AmsiOptions options{.scales = {{"scale1", 1}}};
  Multiscale multiscale(options, mpi);
  auto rank = mpi.scale().rank();
  auto size = mpi.scale().size();
  int failed = 0;
  // allocated zero processes to task
  std::cout << "Creating ProcessSet with 0 processes" << std::endl;
  ProcessSet* ps0 =
      static_cast<ProcessSet*>(new ProcessSet_T<std::pair<int, int> >(-1, -1));
  Task* t0 = new Task(ps0);
  failed += test_neq("Task(0)", static_cast<amsi::Task*>(NULL), t0);
  DataDistribution* dd = createDataDistribution(t0, "not_t0_data");
  failed += test(".verifyDD()", false, t0->verifyDD("t0_data"));
  size_t dd_id0 = t0->getDD_ID("t0_data");
  failed += test_neq(".getDD_ID()", (size_t)0, dd_id0);
  failed += test(".localRank()", -1, t0->localRank());
  failed += test(".localToGlobalRank(-1)", -1, t0->localToGlobalRank(-1));
  failed += test(".localToGlobalRank(0)", -1, t0->localToGlobalRank(0));
  t0->setExecutionFunction(&task0);
  failed += test(".execute()", 0, t0->execute(argc, argv, multiscale));
  (*dd) = rank % 3;
  failed += test(".getLocalDDValue()", -1, (*dd)[t0->localRank()]);
  std::cout << "Creating ProcessSet with 0,size where size = " << size
            << std::endl;
  ProcessSet * ps = static_cast<ProcessSet*>(new ProcessSet_T<std::pair<int, int> >(0,size));
  Task * t1 = new Task(ps);
  failed += test_neq("Task()",static_cast<Task*>(NULL),t1);
  DataDistribution * dd1 = createDataDistribution(t1,"t1_data");
  failed += test(".verifyDD(\"t1_data\")",true,t1->verifyDD("t1_data"));
  failed += test(".verifyDD(\"t1_data_fail\")",false,t1->verifyDD("t1_data_fail"));
  size_t dd_id = t1->getDD_ID("t1_data");
  failed += test_neq(".getDD_ID()",static_cast<size_t>(0),dd_id);
  failed += test(".localRank()",rank,t1->localRank());
  failed += test(".localToGlobalRank()",size >= 4 ? 4 : -1,t1->localToGlobalRank(4));
  t1->setExecutionFunction(&task1);
  failed += test(".execute()", 0, t1->execute(argc, argv, multiscale));
  (*dd1) = rank % 3;
  failed += test(".getLocalDDValue()",rank % 3,(*dd1)[t1->localRank()]);
  return failed;
}
