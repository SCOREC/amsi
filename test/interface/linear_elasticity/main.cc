#include "amsiInterface.h"
#include "simAttributes.h"
#include "Elasticity.h"
#include "Solvers.h"
#include <mpi.h>
#include <cassert>
#include <iostream>
int main (int argc, char ** argv)
{
  assert(argc == 3);
  int result = 0;
  amsi::use_simmetrix = true;
  amsi::use_petsc = true;
  amsi::interfaceInit(argc,argv);
  {
    Sim_logOn("sim.log");
    pGModel mdl = GM_load(argv[1],0,NULL);
    pParMesh msh = PM_load(argv[2],sthreadNone,mdl,NULL);
    std::vector<pACase> css;
    amsi::getTypeCases(SModel_attManager(mdl),"analysis",std::back_inserter(css));
    amsi::initCase(mdl,css[0]);
    pACase pd = (pACase)AttNode_childByType((pANode)css[0],"problem definition");
    amsi::PetscLAS las(0,0);
    amsi::Elasticity iso_lin(mdl,msh,pd,AMSI_COMM_SCALE);
    amsi::LinearSolver(&iso_lin,&las);
    apf::writeVtkFiles("isotropic_linear_elastic_result",
                       iso_lin.getMesh());
    amsi::freeCase(css[0]);
    Sim_logOff();
  } // destroys all stack-allocated objects before deinitialization of libraries
  amsi::interfaceFree();
  return result;
}
