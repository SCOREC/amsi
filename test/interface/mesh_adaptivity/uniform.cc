#include "amsiInterface.h"
#include "NonLinearElastic_UniformAdapt.h"
#include "Solvers.h"
#include <mpi.h>
#include <cassert>
#include <iostream>
int main (int argc, char ** argv)
{
  assert(argc == 3);
  amsi::use_simmetrix = true;
  amsi::interfaceInit(argc,argv);
  Sim_logOn("simmetrix_log");
  {
    pGModel mdl = GM_load(argv[1],0,NULL);
    pParMesh msh = PM_load(argv[2],sthreadNone,mdl,NULL);
    amsi::UniformAdapt fea(mdl,msh,NULL);
    fea.Adapt();
    apf::writeVtkFiles("uniformly_adapted_mesh",fea.getMesh());
    Sim_logOff();
  }
  amsi::interfaceFree();
  return 0;
}
