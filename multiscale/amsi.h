#ifndef AMSI_H_
#define AMSI_H_
#include "amsiMPI.h"
namespace amsi
{
  void amsiInit(int argc, char ** argv, MPI_Comm cm = MPI_COMM_WORLD);
  void amsiFree();
}
#endif
