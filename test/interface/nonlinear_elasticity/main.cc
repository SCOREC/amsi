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
    amsi::buildFieldsFromSim(pd,apf_msh,std::back_inserter(flds));
    std::vector<apf::Numbering*> nms;
    amsi::buildNumberingsFromSim(mdl,flds.begin(),flds.end(),std::back_inserter(nms));;
    std::vector<amsi::SimBCQuery*> bcs[amsi::NUM_BC_TYPES];
    std::vector<pANode> bcs;
    amsi::getTypeNodes((pANode)pd,amsi::getBCTypeString(amsi::DIRICHLET),std::back_inserter(bcs));
    for(auto bc = bcs.begin(); bc != bcs.end(); ++bc)
    {
      amsi::describeNode(*bc);
      std::vector<pANode> fld_nm;
      amsi::getTypeNodes(*bc,"field name",std::back_inserter(fld_nm));
      amsi::describeNode(fld_nm[0]);
    }
    //amsi::buildSimBCQueries(pd,amsi::DIRICHLET,/*amsi::UNITLESS,amsi::NUM_FIELD_UNITS*/,std::back_inserter(bcs[amsi::DIRICHLET]));
    //amsi::buildSimBCQueries(pd,amsi::NEUMANN,/*amsi::CUSTOM,amsi::NUM_NEUMANN_TYPES*/,std::back_inserter(bcs[amsi::NEUMANN]));
    //after building queries, associate them with fields, since we need the geometric entities they lie on anyway, but we also have to relate the SimBCQueries BACK to the attributes taged on the model...
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
