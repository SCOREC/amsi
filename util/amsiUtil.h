#ifndef AMSIUTIL_H_
#define AMSIUTIL_H_
#include "amsiMPI.h"
#include "amsiFileSys.h"
#include "amsiReporter.h"
#include "amsiOutput.h"
#include "amsiUtilConfig.h"
namespace amsi
{
  extern FileSystemInfo * fs;
  void utilInit(int argc, char ** argv, MPI_Comm cm = MPI_COMM_WORLD);
  void utilFree();
}
#endif
