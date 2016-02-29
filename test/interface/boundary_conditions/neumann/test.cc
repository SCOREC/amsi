#include "test.h"
#include "amsiInterface.h"
#include "amsiDummyLAS.h"
#include "simAnalysis.h"
#include "simAttributes.h"
#include "simBoundaryConditions.h"
#include "apfBoundaryConditions.h"
#include <apf.h>
#include <apfNumbering.h>
#include <apfSIM.h>
#include <mpi.h>
#include <cassert>
#include <fstream>
int main(int argc, char ** argv)
{
  int failed = 0;
  assert(argc == 3);
  amsi::use_simmetrix = true;
  amsi::interfaceInit(argc,argv);
  Sim_logOn("simlog");
  pGModel mdl = GM_load(argv[1],0,NULL);
  pParMesh sm_msh = PM_load(argv[2],sthreadNone,mdl,NULL);
  pMesh prt = PM_mesh(sm_msh,0);
  // get all analysis cases
  std::vector<pACase> css;
  amsi::getTypeCases(SModel_attManager(mdl),"analysis",std::back_inserter(css));
  // only run the first attribute case
  amsi::initCase(mdl,css[0]);
  pACase pd = (pACase)AttNode_childByType((pANode)css[0],"problem definition");
  apf::Mesh * msh =  apf::createMesh(sm_msh);
  apf::Field * u = apf::createLagrangeField(msh,"displacement",apf::VECTOR,1);
  apf::Numbering * nm = apf::createNumbering(u);
  int dofs = apf::NaiveOrder(nm);
  amsi::DummyLAS las(dofs);
  int tps[] = {amsi::SURFACE_TRACTION,amsi::NORMAL_PRESSURE};
  std::vector<amsi::SimBCQuery*> neu_qrys;
  amsi::buildSimBCQueries(pd,amsi::NEUMANN,&tps[0],(&tps[0])+1,std::back_inserter(neu_qrys));
  amsi::applySimNeumannBCs(&las,nm,prt,neu_qrys.begin(),neu_qrys.end(),1.0);
  double nrm = 0.0;
  las.GetVectorNorm(nrm);
  failed += test_neq("Force vector norm",0.0,nrm);
  amsi::freeCase(css[0]);
  Sim_logOff();
  amsi::interfaceFree();
  return failed;
}
