#include "amsi.h"

#include "ControlService.h"

#include <TaskManager.h>
#include <CommunicationManager.h>
#include <amsiMeta.h>
#include <amsiInterface.h>

namespace amsi {

  void amsiControlInit::amsiInit(int argc, char ** argv)
  {
    amsiMetaInit::amsiInit(argc,argv);
    if(tm != NULL && cm != NULL)
    {
      ControlService * cs = ControlService::Instance();
      cs->SetTaskManager(tm);
      cs->SetCommunicationManager(cm);
    }
    amsiInterfaceInit::amsiInit(argc,argv);
  }

  void amsiControlInit::amsiFree()
  {
    amsiInterfaceInit::amsiFree();
    amsiMetaInit::amsiFree();
  }

}
