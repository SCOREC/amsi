#ifndef AMSI_H_
#define AMSI_H_
#include "amsiUtil.h"
#include "amsiMeta.h"
#include "amsiInterface.h"
namespace amsi
{
  void controlInit(int argc, char ** argv, MPI_Comm cm = AMSI_COMM_SCALE);
  void controlFree();
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
