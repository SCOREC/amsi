#include "amsiInterface.h"

#include <amsiMPI.h>

#if SIM
#include <gmi_sim.h>
#endif

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
    SimMeshing_start();
#   if HOST==BGQ
    SimField_start();
#   endif
    gmi_sim_start();
    gmi_register_sim();
#   endif
  }

  void amsiInterfaceFree()
  {
#   ifdef SIM
    gmi_sim_stop();
#   if HOST==GBQ
    SimField_stop();
#   endif
    SimMeshing_stop();
    SimPartitionedMesh_stop();
    Sim_unregisterAllKeys();
#   endif

#   ifdef PETSC
    PetscFinalize();
#   endif
    amsiUtilFree();
  }
  
}
