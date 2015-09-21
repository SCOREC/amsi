#ifndef AMSIINTERFACE_H_
#define AMSIINTERFACE_H_

#include "amsiInterfaceConfig.h"
#include "amsiUtil.h"

#ifdef SIM
#include <MeshSim.h>
#include <MeshSimAdapt.h>
#include <SimPartitionedMesh.h>
#include <SimField.h>
#include <SimParallel.h>
#endif

#ifdef PETSC
#include <PetscLAS.h>
#endif

namespace amsi {

  class amsiInterfaceInit : public amsiUtilInit
  {
  public:
    virtual void amsiInit(int,char**);
    virtual void amsiFree();
  };

}

#endif
