#include "test.h"
#include "simAnalysis.h"
#include "simAttributes.h"
#include "simBoundaryConditions.h"
#include "apf.h"
#include "apfSIM.h"
#include "apfBoundaryConditions.h"
#include "amsiAnalysis.h"
#include <cassert>
#include <iterator>
#include <vector>
int main(int argc, char * argv[])
{
  assert(argc == 3);
  int failed = 0;
  amsi::useSimmetrix("/net/common/meshSim/license/license.txt");
  amsi::initAnalysis(argc, argv, MPI_COMM_WORLD);
  Sim_logOn("sim.log");
  // above here taken care of by amsi init
  pPList lst = PList_new();
  void * itm = NULL;
  PList_append(lst,itm);
  std::vector<void*> cpy;
  amsi::cutPaste<void*>(lst,std::back_inserter(cpy));
  failed += test("PList move",(int)cpy.size(),1);
  pGModel mdl = GM_load(argv[1],0,NULL);
  //pParMesh sm_msh = PM_load(argv[2], sthreadNone, mdl, NULL);
  //pMesh sm_prt = PM_mesh(sm_msh,0);
  std::vector<pACase> css;
  amsi::getTypeCases(SModel_attManager(mdl),"analysis",
                     std::back_inserter(css));
  amsi::initCase(mdl,css[0]);
  pACase pd = (pACase)AttNode_childByType((pANode)css[0],"problem definition");
  failed += pd == NULL;
  pACase op = (pACase)AttNode_childByType((pANode)css[0],"output");
  failed += op == NULL;
  pANode frc_op = AttNode_childByType((pANode)op,"output force");
  failed += frc_op == NULL;
  pANode dsp_op = AttNode_childByType((pANode)op,"output displacement");
  failed += dsp_op == NULL;
  std::vector<apf::ModelEntity*> frc_itms;
  std::vector<apf::ModelEntity*> dsp_itms;
  amsi::getAssociatedModelItems(op,frc_op,std::back_inserter(frc_itms));
  amsi::getAssociatedModelItems(op,dsp_op,std::back_inserter(dsp_itms));
  std::vector<apf::ModelEntity*> frc_itms2;
  amsi::getTrackedModelItems(css[0],"output force",std::back_inserter(frc_itms2));
  std::vector<apf::ModelEntity*> dsp_itms2;
  amsi::getTrackedModelItems(css[0],"output displacement",std::back_inserter(dsp_itms2));
  amsi::freeCase(css[0]);
  // below here taken care of by amsi free
  Sim_logOff();
  amsi::freeAnalysis();
  return failed;
}
