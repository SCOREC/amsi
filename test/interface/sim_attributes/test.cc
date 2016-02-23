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
  std::vector<pACase> css;
  amsi::getTypeCases(SModel_attManager(mdl),"analysis",
                     std::back_inserter(css));
  amsi::initCase(mdl,css[0]);
  std::list<pGEntity> ents;
  amsi::getWithAttribute(mdl,"force",ents);
  pAttribute att2 = GEN_attrib(ents.front(),"force");
  pACase pd = (pACase)AttNode_childByType((pANode)css[0],"problem definition");
  std::vector<pANode> bcs;
  amsi::getTypeNodes((pANode)pd,"force",std::back_inserter(bcs));
  std::vector<pModelAssoc> mdl_ascs;
  amsi::cutPaste<pModelAssoc>(AttCase_findModelAssociations(pd,bcs[0]),std::back_inserter(mdl_ascs));
  std::vector<pModelItem> mdl_itms;
  amsi::cutPaste<pModelItem>(AMA_modelItems(mdl_ascs[0]),std::back_inserter(mdl_itms));
  pAttribute att = GEN_attrib((pGEntity)mdl_itms[0],"force");
  std::vector<pAttribute> atts;
  amsi::cutPaste<pAttribute>(Attribute_children(att),std::back_inserter(atts));
  amsi::describeAttribute(atts[0]);
  amsi::freeCase(css[0]);
  // below here taken care of by amsi free
  Sim_logOff();
  SimPartitionedMesh_stop();
  MPI_Finalize();
  return failed;
};
