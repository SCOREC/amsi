#ifndef AMSI_INTERFACE_H_
#define AMSI_INTERFACE_H_
#include "amsiFields.h"
#include "amsiInterfaceConfig.h"
#include "amsiUtil.h"
#include "amsiMPI.h"
#include "amsiPETScLAS.h"
#include "apfFields.h"
#include "simAttributes.h"
#include "simFields.h"
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
