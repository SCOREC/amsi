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
    apf::Mesh * apf_msh = apf::createMesh(msh);
    std::vector<apf::Field*> flds;
    amsi::buildFieldsFromSim(mdl,apf_msh,std::back_inserter(flds));
    std::vector<apf::Numbering*> dof_ids;
    amsi::buildNumberingsFromSim(mdl,flds.begin(),flds.end(),std::back_inserter(dof_ids));
    // each field can have its own dirichlet bcs, but just store them all with a record of which field they are related to
    std::vector<amsi::SimBCQuery*> bcs[amsi::NUM_BC_TYPES];
    for(std::vector<apf::Field*>::iterator fld = flds.begin(); fld != flds.end(); ++fld)
    {
      for(int ii = 0; ii < amsi::NUM_BC_TYPES; ii++)
      {
        std::vector<int> bc_tps;
        amsi::getApplicableBCTypesForField(amsi::decodeFieldType(apf::getName(*fld)),ii,std::back_inserter(bc_tps));
        amsi::buildSimBCQueries(pd,ii,bc_tps.begin(),bc_tps.end(),std::back_inserter(bcs[ii]));
      }
    }
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
