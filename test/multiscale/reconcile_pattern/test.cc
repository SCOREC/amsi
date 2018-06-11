#include "amsiMultiscale.h"
#include "amsiControlService.h"
int task1(int &, char **&, MPI_Comm cm)
{
  amsi::ControlService * cs = amsi::ControlService::Instance();
  amsi::Task * t1 = amsi::getLocal();
  amsi::DataDistribution * dd = amsi::createDataDistribution(t1,"to_t2");
  (*dd) = 4;
  Assemble(dd,cm);
  size_t id = cs->CreateCommPattern("to_t2","task1","task2");
  cs->CommPattern_Reconcile(id);
  return 0;
}
int task2(int &, char **&, MPI_Comm)
{
  amsi::ControlService * cs = amsi::ControlService::Instance();
  size_t id = cs->RecvCommPattern("to_t2","task1","","task2");
  cs->CommPattern_Reconcile(id);
  return 0;
}
int main(int argc, char ** argv)
{
  int failed = 0;
  amsi::initMultiscale(argc,argv, MPI_COMM_WORLD);
  amsi::ControlService * cs = amsi::ControlService::Instance();
  amsi::TaskManager * tm = cs->GetTaskManager();
  amsi::Task * t1 = tm->getTask("task1");
  amsi::Task * t2 = tm->getTask("task2");
  t1->setExecutionFunction(&task1);
  t2->setExecutionFunction(&task2);
  failed += cs->Execute(argc,argv);
  amsi::freeMultiscale();
  return failed;
}
