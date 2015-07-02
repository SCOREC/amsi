#ifndef AMSIINTERFACE_H_
#define AMSIINTERFACE_H_

#include "amsiConfig.h"
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

  void amsiInterfaceInit(int,char**);
  void amsiInterfaceFree();

}

#endif
