#include "amsiAnalysis.h"
#include "apfMeasure.h"
#include <cstdlib>
int main(int argc, char * argv[])
{
  int result = 0;
  amsi::useSimmetrix("/net/common/meshSim/license/license.txt");
  amsi::initAnalysis(argc,argv);
  pGModel mdl = GM_load(argv[1],0,NULL);
  pParMesh msh = PM_load(argv[2],mdl,NULL);
  int tg = atoi(argv[3]);
  apf::Mesh * apf_msh = apf::createMesh(msh);
  apf::Field * u =  apf::createLagrangeField(apf_msh,"u",apf::VECTOR,1);
  apf::zeroField(u);
  pGEntity rgn = (pGRegion)GM_entityByTag(mdl,3,tg);
  double vol = 0.0;
  vol = amsi::measureDisplacedModelEntity_greens(reinterpret_cast<apf::ModelEntity*>(rgn),u);
  int rnk = -1;
  MPI_Comm_rank(MPI_COMM_WORLD,&rnk);
  if(rnk == 0)
    std::cout << vol << std::endl;
  amsi::freeAnalysis();
  return result;
}
