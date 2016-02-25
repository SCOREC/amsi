#include "amsiInterface.h"
#include "NonLinElasticity.h"
#include "Solvers.h"
#include <mpi.h>
#include <cassert>
#include <iostream>
int main (int argc, char ** argv)
{
  int result = 0;
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
    amsi::NonLinElasticity iso_non(mdl,msh,pd);
    double nrm = 0.0;
    amsi::NewtonSolver(&iso_non,&las,30,1e-4,1.0,nrm);
    apf::writeVtkFiles("isotropic_nonlinear_elastic_result",iso_non.getMesh());
    amsi::freeCase(css[0]);
  }
  Sim_logOff();
  amsi::interfaceFree();
  return result;
}
