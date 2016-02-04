#include "amsiUtil.h"
#include "amsiUtilConfig.h"
#include "amsiMPI.h"
#include <iostream>
#ifdef CORE
#include <PCU.h>
#endif
namespace amsi
{
# ifdef CORE
  void pcuInit(int argc, char ** argv, MPI_Comm cm)
  {
    PCU_Comm_Init();
  }
  void pcuFree()
  {
    PCU_Comm_Free();
  }
# endif
  void utilInit(int argc, char ** argv, MPI_Comm cm)
  {
    MPI_Init(&argc,&argv);
    MPI_Comm_dup(cm,&AMSI_COMM_SCALE);
    MPI_Comm_dup(cm,&AMSI_COMM_WORLD);
#   ifdef CORE
    pcuInit(argc,argv,cm);
#   endif
  }
  void utilFree()
  {
    MPI_Finalize();
  }
}
