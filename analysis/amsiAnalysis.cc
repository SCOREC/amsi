#include "amsiInterface.h"
#include "amsiMeta.h"
#include "sim.h"
#include "amsiMPI.h"
#include <gmi_null.h>
#include <gmi_sim.h>
namespace amsi
{
  void interfaceInit(int argc, char ** argv, MPI_Comm cm)
  {
    metaInit(argc,argv,cm);
#   ifdef PETSC
    if(use_petsc)
    {
      if(from_file)
        petscInit(argc,argv,getLocal()->comm());
      else
        petscInit(argc,argv,cm);
    }
#   endif
    if(use_simmetrix)
      simmetrixInit(argc,argv,cm);
    gmi_register_null();
  }
  void interfaceFree()
  {
    if(use_simmetrix)
      simmetrixFree();
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
    gmi_sim_start();
    gmi_register_sim();
  }
  void simmetrixFree()
  {
    gmi_sim_stop();
#   ifdef SCOREC
    SimField_stop();
#   endif
    SimMeshing_stop();
    SimPartitionedMesh_stop();
    Sim_unregisterAllKeys();
  }
}
