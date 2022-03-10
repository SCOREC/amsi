#include "test.h"
#include "amsiMultiscale.h"
#include "amsiControlService.h"
#include "amsiMultiscale.h"
#include "amsiCommunicationManager.h"
#include "amsiTaskManager.h"
int task1(int&, char**&, MPI_Comm, amsi::Multiscale& multiscale)
{
  int fld = 0;
  auto* communication_manager = multiscale.getMultiscaleManager();
  auto* task_manager = multiscale.getScaleManager();
  auto* cs = multiscale.getControlService();
  auto rid = amsi::getRelationID(communication_manager, task_manager, "task1",
                                 "task2");
  std::string tst_str(
      "This is a test of the inter-scale broadcast system. This is only a "
      "test.");
  std::vector<MPI_Request> rqsts;
  cs->aSendBroadcast(
      std::back_inserter(rqsts), rid, tst_str.c_str(),
      tst_str.size() + 1);  // need to account for null terminated c-string
                            // which adds a character
  MPI_Status sts[rqsts.size()];
  MPI_Waitall(rqsts.size(), &rqsts[0], &sts[0]);
  return fld;
}
int task2(int&, char**&, MPI_Comm, amsi::Multiscale& multiscale)
{
  int fld = 0;
  auto* communication_manager = multiscale.getMultiscaleManager();
  auto* task_manager = multiscale.getScaleManager();
  auto* cs = multiscale.getControlService();
  auto rid = amsi::getRelationID(communication_manager, task_manager, "task1",
                                 "task2");
  MPI_Request rqst;
  char* rcv_bfr = NULL;
  size_t sz = 0;
  while ((sz = cs->aRecvBroadcastSize<char>(rid)) == 0) {
    // process other stuff
  }
  rcv_bfr = new char[sz];
  cs->aRecvBroadcast(&rqst, rid, rcv_bfr, sz);
  MPI_Status sts;
  MPI_Wait(&rqst, &sts);
  std::string tst_str(rcv_bfr);
  std::cout << tst_str << std::endl;
  return fld;
}
int main(int argc, char * argv[])
{
  int fld = 0;
  amsi::MPI mpi(argc, argv);
  amsi::AmsiOptions options{
      .scales = {{"task1", 8}, {"task2", 24}},
      .relations = {{"task1", "task2"}, {"task2", "task1"}},
      .results_directory = "results"};
  amsi::Multiscale multiscale(options, mpi);
  auto* t1 = multiscale.getScaleManager()->getTask("task1");
  auto* t2 = multiscale.getScaleManager()->getTask("task2");
  auto* cs = multiscale.getControlService();
  t1->setExecutionFunction(&task1);
  t2->setExecutionFunction(&task2);
  fld += cs->Execute(argc, argv, multiscale);
  test("Tests failed", 0, fld);
  return fld;
}
