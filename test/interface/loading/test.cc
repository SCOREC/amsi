#include "amsiInterface.h"
#include <cassert>
int main(int argc, char * argv[])
{
  assert(argc == 3);
  amsi::use_simmetrix = true;
  amsi::interfaceInit(argc,argv);
  Sim_logOn("simmetrix_log");
  pGModel mdl = GM_load(argv[1],NULL,NULL);
  pParMesh msh = PM_load(argv[2],sthreadNone,mdl,NULL);
  Sim_logOff();
  amsi::interfaceFree();
  return 0;
}
