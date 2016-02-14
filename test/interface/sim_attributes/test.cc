#include "test.h"
#include "simAnalysis.h"
#include "simAttributes.h"
#include "simBoundaryConditions.h"
#include "apf.h"
#include "apfSIM.h"
#include "apfBoundaryConditions.h"
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
  apf::Mesh * msh = apf::createMesh(sm_msh);
  apf::Field * u = apf::createLagrangeField(msh,"displacement",apf::VECTOR,1);
  apf::Numbering * nm = apf::createNumbering(u);
  std::vector<pACase> css;
  amsi::getTypeCases(SModel_attManager(mdl),"analysis",
                     std::back_inserter(css));
  failed += test("getTypeCases()",1,(int)css.size());
  amsi::initCase(mdl,css[0]);
  std::vector<pACase> pd;
  amsi::cutPaste<pACase>(AttCase_findChildrenByType(css[0],"problem definition"),
                         std::back_inserter(pd));
  failed += test("problem definition",1,(int)pd.size());
  std::vector<amsi::SimBC*> dir_bcs;
  amsi::buildDirichletBCs(pd[0],std::back_inserter(dir_bcs));
  failed += test("buildDirichletBCs()",1,(int)dir_bcs.size());
  failed += test("dirichlet type",(int)amsi::DISPLACEMENT,dir_bcs[0]->sbtp);
  int dsp[] = {amsi::DISPLACEMENT};
  std::vector<amsi::SimBC*> dir_bcs2;
  amsi::buildBCs(pd[0],amsi::DIRICHLET,dsp,dsp+1,std::back_inserter(dir_bcs2));
  std::vector<pAttribute> atts;
  amsi::getDirichletBCAttributes(dir_bcs[0],std::back_inserter(atts));
  failed += test("getDirichletAttributes()",3,(int)atts.size());
  //amsi::applyBC(dir_bcs[0],sm_prt);
  for(auto bc : dir_bcs)
  {
    if(bc->sbtp == amsi::DISPLACEMENT)
    {
      amsi::SimDisplacementQuery spc(bc);
      std::list<pEntity> ents;
      int dm = amsi::modelItemTypeDim(GEN_type((pGEntity)bc->itm));
      for(int d = dm; d >= 0; --d)
        amsi::getClassifiedEnts(sm_prt,(pGEntity)bc->itm,d,std::back_inserter(ents));
      amsi::applyDirichletBC(u,nm,ents.begin(),ents.end(),&spc,0.0);
    }
  }
  std::vector<amsi::SimBC*> neu_bcs;
  int frc[] = {amsi::SURFACE_TRACTION, amsi::PRESSURE};
  amsi::buildBCs(pd[0],amsi::NEUMANN,frc,frc+2,std::back_inserter(neu_bcs));
  for(auto bc : neu_bcs)
  {
    amsi::BCQuery * qry = NULL;
    if(bc->sbtp == amsi::SURFACE_TRACTION)
      qry = new amsi::SimTensor1Query(bc);
    else if(bc->sbtp == amsi::PRESSURE)
      qry = new amsi::SimTensor0Query(bc);
    assert(qry);
    // build the correct integrator instead of the query above...
    // then process all the model entities
  }
  amsi::freeCase(css[0]);
  // below here taken care of by amsi free
  Sim_logOff();
  SimPartitionedMesh_stop();
  MPI_Finalize();
  return failed;
};
