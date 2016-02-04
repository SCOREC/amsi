#include "amsi.h"
#include "amsiControlService.h"
#include <TaskManager.h>
#include <CommunicationManager.h>
#include <amsiMeta.h>
#include <amsiInterface.h>
namespace amsi
{
  void controlInit(int argc, char ** argv, MPI_Comm cm)
  {
    ControlService * cs = ControlService::Instance();
    cs->SetTaskManager(amsi::tm);
    cs->SetCommunicationManager(amsi::cm);
  }
  void controlFree()
  { }
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
