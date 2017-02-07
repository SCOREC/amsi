#include "amsiAnalysis.h"
#include "apfsimWrapper.h"
#include "simWrapper.h"
#include <cstdlib>
int main(int argc, char * argv[])
{
  int result = 0;
  amsi::useSimmetrix("/net/common/meshSim/license/license.txt");
  amsi::initAnalysis(argc,argv);
  pGModel mdl = GM_load(argv[1],0,NULL);
  pParMesh msh = PM_load(argv[2],sthreadNone,mdl,NULL);
  int tg = atoi(argv[3]);
  pMesh prt = PM_mesh(msh,0);
  apf::Mesh * apf_msh = apf::createMesh(msh);
  apf::Field * u =  apf::createLagrangeField(apf_msh,"u",apf::VECTOR,1);
  apf::zeroField(u);
  pGRegion rgn = (pGRegion)GM_entityByTag(mdl,3,tg);
  std::vector<pGEntity> fcs;
  amsi::cutPaste<pGEntity>(GEN_faces((pGEntity)rgn),std::back_inserter(fcs));
  double vol = 0.0;
  for(std::vector<pGEntity>::iterator fc = fcs.begin(); fc != fcs.end(); ++fc)
    vol += amsi::measureVol_pGFace((pGFace)*fc,tg,prt,u);
  int rnk = -1;
  MPI_Comm_rank(MPI_COMM_WORLD,&rnk);
  if(rnk == 0)
    std::cout << vol << std::endl;
  amsi::freeAnalysis();
  return result;
}
