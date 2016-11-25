#include "amsiUtil.h"
#include "amsiUtilConfig.h"
#include "amsiMPI.h"
#include <iostream>
#include <PCU.h>
namespace amsi
{
  FileSystemInfo * fs = NULL;
  void initUtil(int argc, char ** argv, MPI_Comm cm)
  {
    MPI_Init(&argc,&argv);
    MPI_Comm_dup(cm,&AMSI_COMM_SCALE);
    MPI_Comm_dup(cm,&AMSI_COMM_WORLD);
    PCU_Comm_Init();
  }
  void freeUtil()
  {
    PCU_Comm_Free();
    MPI_Finalize();
  }
}
