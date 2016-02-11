#include "test.h"
#include "simAnalysis.h"
#include "simAttributes.h"
#include "simBoundaryConditions.h"
#include <cassert>
#include <iterator>
#include <vector>
int main(int argc, char * argv[])
{
  assert(argc == 3);
  int failed = 0;
  MPI_Init(&argc,&argv);
  Sim_readLicenseFile("/net/common/meshSim/license/license.txt");
  SimPartitionedMesh_start(NULL,NULL);
  Sim_logOn("sim.log");
  // above here taken care of by amsi init
  pGModel mdl = GM_load(argv[1],0,NULL);
  pParMesh sm_msh = PM_load(argv[2], sthreadNone, mdl, NULL);
  pMesh sm_prt = PM_mesh(sm_msh,0);
  std::vector<pACase> css;
  amsi::getTypeCases(SModel_attManager(mdl),"analysis",
                     std::back_inserter(css));
  failed += test("getTypeCases()",1,(int)css.size());
  amsi::initCase(mdl,css[0]);
  std::vector<pACase> pd;
  amsi::cutPaste<pACase>(AttCase_findChildrenByType(css[0],"problem definition"),
                         std::back_inserter(pd));
  failed += test("problem definition",1,(int)pd.size());
  std::vector<amsi::SimDirichletBC*> dir_bcs;
  amsi::buildDirichletBCs(pd[0],std::back_inserter(dir_bcs));
  failed += test("buildDirichletBCs()",1,(int)dir_bcs.size());
  failed += test("dirichlet type",amsi::dir_bc_attrs[0],dir_bcs[0]->dir_tp);
  std::vector<pAttribute> atts;
  amsi::getDirichletAttributes(dir_bcs[0],std::back_inserter(atts));
  failed += test("getDirichletAttributes()",3,(int)atts.size());
  amsi::applyDirichletBC(dir_bcs[0],sm_prt);
  amsi::freeCase(css[0]);
  // below here taken care of by amsi free
  Sim_logOff();
  SimPartitionedMesh_stop();
  MPI_Finalize();
  return failed;
};
