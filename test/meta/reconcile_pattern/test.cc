#include "amsiControl.h"
#include "amsiControlService.cc"
#include "amsiInterface.h"
int task1(int & argc, char **& argv, MPI_Comm cm)
{
  amsi::ControlService * cs = amsi::ControlService::Instance();
  amsi::Task * t1 = amsi::getLocal();
  t1->createDD("to_t2");
  t1->setLocalDDValue("to_t2",4);
  t1->assembleDD("to_t2");
  size_t id = cs->CreateCommPattern("to_t2","task1","task2");
  cs->CommPattern_Reconcile(id);
  return 0;
}
int task2(int & argc, char **& argv, MPI_Comm cm)
{
  amsi::ControlService * cs = amsi::ControlService::Instance();
  size_t id = cs->RecvCommPattern("to_t2","task1","","task2");
  cs->CommPattern_Reconcile(id);
  return 0;
}
int main(int argc, char ** argv)
{
  int failed = 0;
  amsi::use_petsc = false;
  amsi::use_simmetrix = false;
  amsi::controlInit(argc,argv);
  amsi::ControlService * cs = amsi::ControlService::Instance();
  cs->setSuppressOutput(false);
  amsi::TaskManager * tm = cs->GetTaskManager();
  amsi::Task * t1 = tm->getTask("task1");
  amsi::Task * t2 = tm->getTask("task2");
  t1->setExecutionFunction(&task1);
  t2->setExecutionFunction(&task2);
  failed += cs->Execute(argc,argv);
  amsi::controlFree();
  return failed;
}
