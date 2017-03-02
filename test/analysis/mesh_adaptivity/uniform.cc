#include "amsiAnalysis.h"
#include "NonLinearElastic_UniformAdapt.h"
#include "Solvers.h"
#include <mpi.h>
#include <cassert>
#include <iostream>
int main (int argc, char ** argv)
{
  assert(argc == 3);
  amsi::useSimmetrix("/net/common/meshSim/license/license.txt");
  amsi::initAnalysis(argc,argv);
  Sim_logOn("simmetrix_log");
  {
    pGModel mdl = GM_load(argv[1],0,NULL);
    pParMesh msh = PM_load(argv[2],mdl,NULL);
    amsi::UniformAdapt fea(mdl,msh,NULL);
    fea.Adapt();
    apf::writeVtkFiles("uniformly_adapted_mesh",fea.getMesh());
    Sim_logOff();
  }
  amsi::freeAnalysis();
  return 0;
}
