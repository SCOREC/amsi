#include "test.h"
#include "simAttributes.h"
#include <cassert>
int main(int argc, char * argv[])
{
  assert(argc >= 4);
  MPI_Init(&argc,&argv);
  Sim_readLicenseFile("/net/common/meshSim/license/license.txt");
  SimPartitionedMesh_start(NULL,NULL);
  pGModel mdl = GM_load(argv[1],0,NULL);
  pParMesh msh = PM_load(argv[2],sthreadNone,mdl,NULL);
  amsi::initAttributeCase(mdl,argv[3]);
  SimPartitionedMesh_stop();
};
