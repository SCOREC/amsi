#ifndef AMSI_H_
#define AMSI_H_

#include "amsiUtil.h"
#include "amsiMeta.h"
#include "amsiInterface.h"

namespace amsi {

  class amsiControlInit : public amsiMetaInit, public amsiInterfaceInit
  {
  public:
  amsiControlInit()
   : amsiMetaInit(),
     amsiInterfaceInit()
  {}

    virtual void amsiInit(int,char**);
    virtual void amsiFree();
    
  };
  
}

#endif
