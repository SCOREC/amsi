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
  amsi::use_petsc = true;
  amsi::interfaceInit(argc,argv);
  Sim_logOn("sim.log");
  {
    pGModel mdl = GM_load(argv[1],0,NULL);
    pParMesh msh = PM_load(argv[2],sthreadNone,mdl,NULL);
    std::vector<pACase> css;
    amsi::getTypeCases(SModel_attManager(mdl),"analysis",std::back_inserter(css));
    amsi::initCase(mdl,css[0]);
    pACase pd = (pACase)AttNode_childByType((pANode)css[0],"problem definition");
    amsi::PetscLAS las(0,0);
    amsi::UniformAdapt fea(mdl,msh,pd);
    //XFdouble nrm = 0.0;
    //currently fails during the second adaptation when retreiving dofgroups
    //NewtonSolver(fea,las,30,1e-8,1.0,nrm);
    apf::writeVtkFiles("error_estimation",fea.getMesh());
    amsi::freeCase(css[0]);
  }
  Sim_logOff();
  amsi::interfaceFree();
  return 0;
}
