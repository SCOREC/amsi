#ifndef AMSIUTIL_H_
#define AMSIUTIL_H_
#include "amsiMPI.h"
#include "amsiUtilConfig.h"
namespace amsi
{
  void utilInit(int argc, char ** argv, MPI_Comm cm = MPI_COMM_WORLD);
  void utilFree();
# ifdef CORE
  void pcuInit(int argc, char ** argv, MPI_Comm cm = MPI_COMM_WORLD);
  void pcuFree();
# endif
}

#endif
