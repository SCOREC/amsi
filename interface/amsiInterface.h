#ifndef AMSIINTERFACE_H_
#define AMSIINTERFACE_H_

#include "amsiInterfaceConfig.h"
#include "amsiUtil.h"
#include "simAttributes.h"

#ifdef PETSC
#include <PetscLAS.h>
#endif

namespace amsi {

  class amsiInterfaceInit : virtual public amsiUtilInit
  {
  public:
    virtual void amsiInit(int,char**);
    virtual void amsiFree();
  };

}

#endif
