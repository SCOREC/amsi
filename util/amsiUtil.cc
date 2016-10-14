#include "amsiUtil.h"
#include "amsiUtilConfig.h"
#include "amsiMPI.h"
#include <iostream>
#include <PCU.h>
namespace amsi
{
  void pcuInit(int argc, char ** argv, MPI_Comm cm)
  {
    PCU_Comm_Init();
  }
  void pcuFree()
  {
    PCU_Comm_Free();
  }
  void utilInit(int argc, char ** argv, MPI_Comm cm)
  {
    MPI_Init(&argc,&argv);
    MPI_Comm_dup(cm,&AMSI_COMM_SCALE);
    MPI_Comm_dup(cm,&AMSI_COMM_WORLD);
    pcuInit(argc,argv,cm);
  }
  void utilFree()
  {
    MPI_Finalize();
  }
}
