#include "amsi.h"
#include "amsiControl.h"
namespace amsi
{
  void amsiInit(int argc, char ** argv, MPI_Comm cm)
  {
    controlInit(argc,argv,cm);
  }
  void amsiFree()
  {
    controlFree();
  }
}
