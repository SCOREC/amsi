#include "test.h"
#include "amsiInterface.h"
#include "amsiMeta.h"
#include <cassert>
int task(int & argc, char **& argv, MPI_Comm)
{
  int failed = 0;
  Sim_logOn("simmetrix_log");
  pGModel mdl = GM_load(argv[1],NULL,NULL);
  failed += test_neq("GM_load()",(void*)NULL,(void*)mdl);
  pParMesh msh = PM_load(argv[2],sthreadNone,mdl,NULL);
  failed += test_neq("PM_load()",(void*)NULL,(void*)msh);
  Sim_logOff();
  return failed;
}
int main(int argc, char * argv[])
{
  int failed = 0;
  assert(argc == 3);
  amsi::use_simmetrix = true;
  amsi::interfaceInit(argc,argv);
  amsi::TaskManager tm(AMSI_COMM_WORLD);
  amsi::Task * tsk = tm.createTask("task",2);
  tsk->setExecutionFunction(&task);
  failed += tm.Execute(argc,argv);
  amsi::interfaceFree();
  return failed;
}
