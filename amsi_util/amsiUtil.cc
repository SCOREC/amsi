#include "amsiUtil.h"
#include "amsiConfig.h"

#include <mpi.h>

#ifdef CORE
#include <PCU.h>
#endif

MPI_Comm AMSI_COMM_WORLD = MPI_COMM_WORLD;
MPI_Comm AMSI_COMM_LOCAL = MPI_COMM_WORLD;

namespace amsi
{
  static int util_init_call_count = 0;

  void amsiUtilInit(int argc, char ** argv)
  {
    if(!util_init_call_count)
    {
      MPI_Init(&argc,&argv);
#     ifdef CORE
      PCU_Comm_Init();
#    endif
    }
    util_init_call_count++;
  }

  void amsiUtilFree()
  {
    if(util_init_call_count == 1)
    {
#     ifdef CORE
      PCU_Comm_Free();
#     endif
      MPI_Finalize();
    }
    util_init_call_count--;
  }
}
