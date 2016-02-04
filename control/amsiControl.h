#ifndef AMSI_CONTROL_H_
#define AMSI_CONTROL_H_
#include "amsiMPI.h"
namespace amsi
{
  void controlInit(int argc, char ** argv, MPI_Comm cm = MPI_COMM_WORLD);
  void controlFree();
}
#endif
