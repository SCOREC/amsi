#include <amsiAnalysis.h>
#include <apfSIM.h>
int main(int argc, char * argv[])
{
  amsi::useSimmetrix("/net/common/meshSim/license/license.txt");
  amsi::initAnalysis(argc,argv, MPI_COMM_WORLD);
  Sim_logOn("simmetrix_log");
  pParMesh msh = PM_load(argv[1],NULL,NULL);
  apf::Mesh * apfmsh = apf::createMesh(msh);
  apf::writeVtkFiles(argv[2],apfmsh);
  apf::destroyMesh(apfmsh);
  Sim_logOff();
  amsi::freeAnalysis();
}
