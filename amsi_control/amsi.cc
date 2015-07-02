#include "amsi.h"

#include "ControlService.h"

#include <TaskManager.h>
#include <CommunicationManager.h>
#include <amsiMeta.h>
#include <amsiInterface.h>

namespace amsi {

  void amsiInit(int argc, char ** argv)
  {
    amsiMetaInit(argc,argv);
    if(tm != NULL && cm != NULL)
    {
      ControlService * cs = ControlService::Instance();
      cs->SetTaskManager(tm);
      cs->SetCommunicationManager(cm);
    }
    amsiInterfaceInit(argc,argv);
  }

  void amsiFree()
  {
    amsiInterfaceFree();
    amsiMetaFree();
  }

}
