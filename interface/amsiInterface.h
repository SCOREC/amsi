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
  void petscInit(int argc, char ** argv, MPI_Comm cm = AMSI_COMM_SCALE);
  void petscFree();
  void simmetrixInit(int argc, char ** argv, MPI_Comm cm = AMSI_COMM_SCALE);
  void simmetrixFree();
  void interfaceInit(int argc, char ** argv, MPI_Comm cm = AMSI_COMM_SCALE);
  void interfaceFree();
  class amsiInterfaceInit : virtual public amsiUtilInit
  {
  public:
    virtual void amsiInit(int,char**);
    virtual void amsiFree();
  };
}
#endif
