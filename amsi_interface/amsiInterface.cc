#include "amsiInterface.h"

#include <amsiMPI.h>

namespace amsi {

  void amsiInterfaceInit(int argc, char ** argv)
  {
    amsiUtilInit(argc,argv);

    // todo: only initialize libraries on those scales which make use of their functionality
#   ifdef PETSC
    PETSC_COMM_WORLD = AMSI_COMM_LOCAL;
    PetscInitialize(&argc,&argv,"petsc_options",PETSC_NULL);
#   endif

#   ifdef SIM
    PMU_setCommunicator(AMSI_COMM_LOCAL);
    Sim_readLicenseFile("/net/common/meshSim/license/license.txt");
    SimPartitionedMesh_start(NULL,NULL);
    SimModel_start();
    SimField_start();
#   endif
  }

  void amsiInterfaceFree()
  {
#   ifdef SIM
    SimField_stop();
    SimModel_stop();
    SimPartitionedMesh_stop();
    Sim_unregisterAllKeys();
#   endif

#   ifdef PETSC
    PetscFinalize();
#   endif
    amsiUtilFree();
  }
  
}
