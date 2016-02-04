#ifndef AMSIINTERFACE_H_
#define AMSIINTERFACE_H_
#include "amsiInterfaceConfig.h"
#include "amsiUtil.h"
#include "simAttributes.h"
#include "amsiMPI.h"
#ifdef PETSC
#include <PetscLAS.h>
#endif
namespace amsi
{
  void interfaceInit(int argc, char ** argv, MPI_Comm cm = MPI_COMM_WORLD);
  void interfaceFree();
# ifdef PETSC
  void petscInit(int argc, char ** argv, MPI_Comm cm = MPI_COMM_WORLD);
  void petscFree();
  extern bool use_petsc;
# endif
# ifdef SIM
  void simmetrixInit(int argc, char ** argv, MPI_Comm cm = MPI_COMM_WORLD);
  void simmetrixFree();
  extern bool use_simmetrix;
# endif
}
#endif
