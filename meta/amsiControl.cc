#include "amsiControl.h"
#include "amsiControlService.h"
#include "amsiInterface.h"
#include "amsiMeta.h"
namespace amsi
{
  void controlInit(int argc, char ** argv, MPI_Comm cm)
  {
    interfaceInit(argc,argv,cm);
    ControlService * cs = ControlService::Instance();
    cs->SetTaskManager(amsi::tm);
    cs->SetCommunicationManager(amsi::cm);
  }
  void controlFree()
  {
    interfaceFree();
  }
}
