#include <amsiAnalysis.h>
#include <apfsimWrapper.h>
#include <apfSIM.h>
int main(int argc, char * argv[])
{
  amsi::useSimmetrix("/net/common/meshSim/license/license.txt");
  amsi::initAnalysis(argc,argv, MPI_COMM_WORLD);
  Sim_logOn("simmetrix_log");
  pGModel mdl = GM_load(argv[1],NULL,NULL);
  pParMesh msh = PM_load(argv[2],mdl,NULL);
  apf::Mesh * apfmsh = apf::createMesh(msh);
  amsi::applyUniqueRegionTags(mdl,PM_mesh(msh,0),apfmsh);
  apf::writeVtkFiles(argv[3],apfmsh);
  apf::destroyMesh(apfmsh);
  Sim_logOff();
  amsi::freeAnalysis();
}
