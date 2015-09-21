#include "amsiUtil.h"
#include "amsiUtilConfig.h"

#ifdef CORE
#include <PCU.h>
#endif

MPI_Comm AMSI_COMM_WORLD = MPI_COMM_WORLD;
MPI_Comm AMSI_COMM_LOCAL = MPI_COMM_WORLD;

namespace amsi
{
  amsiInitializer * amsi_init = NULL;

  void amsiInit(int argc, char ** argv)
  {
    amsi_init->amsiInit(argc,argv);
  }

  void amsiFree()
  {
    amsi_init->amsiFree();
    delete amsi_init;
  }
  
  void amsiUtilInit::amsiInit(int argc, char ** argv)
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

  void amsiUtilInit::amsiFree()
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
