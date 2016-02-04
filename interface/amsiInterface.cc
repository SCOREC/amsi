#include "amsiInterface.h"
#include "amsiMeta.h"
#include "sim.h"
#include "amsiMPI.h"
#ifdef CORE
#include <gmi_null.h>
#ifdef SIM
#include <gmi_sim.h> // ifdef sim
#endif
#endif
namespace amsi
{
  void interfaceInit(int argc, char ** argv, MPI_Comm cm)
  {
    metaInit(argc,argv,cm);
#   ifdef PETSC
    if(use_petsc)
      petscInit(argc,argv,cm);
#   endif
#   ifdef SIM
    if(use_simmetrix)
      simmetrixInit(argc,argv,cm);
#   elif defined CORE
    gmi_start();
#   endif
#   ifdef CORE
    gmi_register_null();
#   endif
  }
  void interfaceFree()
  {
#   ifdef SIM
    if(use_simmetrix)
      simmetrixFree();
#   endif
#   ifdef PETSC
    if(use_petsc)
      petscFree();
#   endif
    metaFree();
  }
# ifdef PETSC
  bool use_petsc = false;
  void petscInit(int argc, char ** argv, MPI_Comm cm)
  {
    PETSC_COMM_WORLD = cm;
    PetscInitialize(&argc,&argv,"petsc_options",PETSC_NULL);
  }
  void petscFree()
  {
    PetscFinalize();
  }
# endif
# ifdef SIM
  bool use_simmetrix = false;
  void simmetrixInit(int argc, char ** argv, MPI_Comm cm)
  {
    PMU_setCommunicator(AMSI_COMM_SCALE);
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
#   ifdef SIM
    gmi_sim_start();
    gmi_register_sim();
#   endif
  }
  void simmetrixFree()
  {
#   ifdef CORE
    gmi_sim_stop();
#   endif
#   ifdef SCOREC
    SimField_stop();
#   endif
    SimMeshing_stop();
    SimPartitionedMesh_stop();
    Sim_unregisterAllKeys();
  }
# endif
}
