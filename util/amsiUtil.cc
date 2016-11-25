#include "amsiUtil.h"
#include "amsiUtilConfig.h"
#include "amsiMPI.h"
#include <iostream>
#include <PCU.h>
namespace amsi
{
  FileSystemInfo * fs = NULL;
  static int inited = 0;
  void initUtil(int argc, char ** argv, MPI_Comm cm)
  {
    if(!inited)
    {
      MPI_Init(&argc,&argv);
      MPI_Comm_dup(cm,&AMSI_COMM_SCALE);
      MPI_Comm_dup(cm,&AMSI_COMM_WORLD);
      PCU_Comm_Init();
    }
    inited++;
  }
  void freeUtil()
  {
    inited--;
    if(!inited)
    {
      PCU_Comm_Free();
      MPI_Finalize();
    }
  }
}
