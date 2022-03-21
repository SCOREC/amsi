#include "amsi.h"
#include "amsiControlService.h"
#include "amsiMultiscale.h"
int task1(int&, char**&, MPI_Comm cm, amsi::Multiscale& multiscale)
{
  auto* cs = multiscale.getControlService();
  auto* t1 = multiscale.getScaleManager()->getLocalTask();
  amsi::DataDistribution* dd = amsi::createDataDistribution(t1, "to_t2");
  (*dd) = 4;
  // does the task need to be passed a comm? or does it always operate over
  // COMM_SCALE?
  Assemble(dd, cm);
  size_t id = cs->CreateCommPattern("to_t2", "task1", "task2");
  cs->CommPattern_Reconcile(id);
  return 0;
}
int task2(int&, char**&, MPI_Comm, amsi::Multiscale& multiscale)
{
  auto* cs = multiscale.getControlService();
  size_t id = cs->RecvCommPattern("to_t2", "task1", "", "task2");
  cs->CommPattern_Reconcile(id);
  return 0;
}
int main(int argc, char** argv)
{
  amsi::MultiscaleOptions options{.scales = {{"task1", 8}, {"task2", 24}},
                                  .relations = {{"task1", "task2"}}};
  amsi::MPI mpi(argc, argv, MPI_COMM_WORLD);
  amsi::Multiscale multiscale(options, mpi);
  int failed = 0;
  auto* task_manager = multiscale.getScaleManager();
  amsi::Task* t1 = task_manager->getTask("task1");
  amsi::Task* t2 = task_manager->getTask("task2");
  t1->setExecutionFunction(&task1);
  t2->setExecutionFunction(&task2);
  failed += multiscale.getControlService()->Execute(argc, argv, multiscale);
  return failed;
}
