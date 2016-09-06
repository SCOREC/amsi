#include "apfsimWrapper.h"
#include "simWrapper.h"
#include <cstdlib>
int main(int agrc, char * argv[])
{
  int result = 0;
  amsi::use_simmetrix = true;
  amsi::interfaceInit(argc,argv);
  pGModel mdl = GM_load(argv[1],0,NULL);
  pParMesh msh = PM_load(argv[2],sthreadnone,mdl,NULL);
  int tg = atoi(argv[3]);
  pMesh prt = PM_mesh(msh,0);
  apf::Field * u =  apf::createLagrangeField(msh,"u",apf::VECTOR,1);
  pGRegion rgn = (pGRegion)GM_entityByTag(mdl,3,tg);
  std::vector<pGEntity> fcs;
  amsi::cutPaste<pGEntity>(GEN_faces((pGEntity)rgn),fcs);
  double vol = 0.0;
  for(std::vector<pGEntity>::iterator fc = fcs.begin(); fc != fcs.end(); ++fc)
    vol += measureVol_pGFace((pGFace)*fc,tg,prt,u);
  int rnk = -1;
  MPI_Comm_rank(MPI_COMM_WORLD,&rnk);
  if(rnk == 0)
    std::cout << vol << std::endl;
  amsi::interfaceFree();
  return result;
}
