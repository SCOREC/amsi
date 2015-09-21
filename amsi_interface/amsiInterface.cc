#include "amsiInterface.h"

#include <amsiMPI.h>

#ifdef SIM
#include <gmi_sim.h>
#endif

namespace amsi {

  void amsiInterfaceInit::amsiInit(int argc, char ** argv)
  {
    amsiUtilInit::amsiInit(argc,argv);

    // todo: only initialize libraries on those scales which make use of their functionality
#   ifdef PETSC
    PETSC_COMM_WORLD = AMSI_COMM_LOCAL;
    PetscInitialize(&argc,&argv,"petsc_options",PETSC_NULL);
#   endif

#   ifdef SIM
    PMU_setCommunicator(AMSI_COMM_LOCAL);

#   ifdef BGQ
    Sim_readLicenseFile("/gpfs/u/software/bgq/proprietary/simmetrix//license/license.txt");
#   elif defined SCOREC
    Sim_readLicenseFile("/net/common/meshSim/license/license.txt");
#   endif
    SimPartitionedMesh_start(NULL,NULL);
    SimMeshing_start();
#   ifdef SCOREC
    SimField_start();
#   endif
    gmi_sim_start();
    gmi_register_sim();
#   endif
  }

  void amsiInterfaceInit::amsiFree()
  {
#   ifdef SIM
    gmi_sim_stop();
#   ifdef SCOREC
    SimField_stop();
#   endif
    SimMeshing_stop();
    SimPartitionedMesh_stop();
    Sim_unregisterAllKeys();
#   endif

#   ifdef PETSC
    PetscFinalize();
#   endif
    amsiUtilInit::amsiFree();
  }
  
}
