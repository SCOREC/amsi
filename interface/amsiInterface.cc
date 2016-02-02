#include "amsiInterface.h"
#include <amsiMPI.h>
#include <gmi_null.h>
#include <gmi_sim.h> // ifdef sim
namespace amsi {
  bool use_petsc = true;
  void amsiInterfaceInit::amsiInit(int argc, char ** argv)
  {
    amsiUtilInit::amsiInit(argc,argv);
    if(use_petsc)
    {
      PETSC_COMM_WORLD = AMSI_COMM_LOCAL;
      PetscInitialize(&argc,&argv,"petsc_options",PETSC_NULL);
    }
    if(use_simmetrix)
    {
      PMU_setCommunicator(AMSI_COMM_LOCAL);
#     ifdef BGQ
      Sim_readLicenseFile("/gpfs/u/software/bgq/proprietary/simmetrix//license/license.txt");
#     elif defined SCOREC
      Sim_readLicenseFile("/net/common/meshSim/license/license.txt");
#     endif
      SimPartitionedMesh_start(NULL,NULL);
      SimMeshing_start();
#     ifdef SCOREC
      SimField_start();
#     endif
      gmi_sim_start();
      gmi_register_sim();
    }
    gmi_register_null();
  }
  void amsiInterfaceInit::amsiFree()
  {
#   ifdef SIM
    gmi_sim_stop();
#    ifdef SCOREC
    SimField_stop();
#    endif
    SimMeshing_stop();
    SimPartitionedMesh_stop();
    Sim_unregisterAllKeys();
#   endif
#   ifdef PETSC
  if(use_petsc)
    PetscFinalize();
#   endif
    amsiUtilInit::amsiFree();
  }
}
