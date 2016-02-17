#include "test.h"
#include "amsiInterface.h"
#include "simAnalysis.h"
#include "simAttributes.h"
#include "simBoundaryConditions.h"
#include "apf.h"
#include "apfSIM.h"
#include "apfBoundaryConditions.h"
#include <mpi.h>
#include <cassert>
#include <fstream>
int main(int argc, char ** argv)
{
  int failed = 0;
  assert(argc == 3);
  amsi::use_simmetrix = true;
  amsi::interfaceInit(argc,argv);
  pGModel mdl = GM_load(argv[1],0,NULL);
  pParMesh sm_msh = PM_load(argv[2],sthreadNone,mdl,NULL);
  // get all analysis cases
  std::vector<pACase> css;
  amsi::getTypeCases(SModel_attManager(mdl),"analysis",std::back_inserter(css));
  // only run the first attribute case
  amsi::initCase(mdl,css[0]);
  pACase pd = (pACase)AttNode_childByType((pANode)css[0],"problem definition");
  int dsp[] = {amsi::DISPLACEMENT};
  // wrap below here in function ?
  std::vector<amsi::SimBC*> dir_bcs;
  amsi::buildBCs(pd,amsi::DIRICHLET,dsp,dsp+1,std::back_inserter(dir_bcs));
  for(auto dir_bc : dir_bcs)
  {
    amsi::BCQuery * qry = amsi::buildSimBCQuery(dir_bc);
  }
  
  amsi::freeCase(css[0]);
  apf::Mesh * msh =  apf::createMesh(sm_msh);
  apf::Field * u = apf::createLagrangeField(msh,"displacement",apf::VECTOR,1);
  apf::Numbering * nm = apf::createNumbering(u);
  return failed;
}
