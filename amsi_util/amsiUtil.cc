#include "amsiUtil.h"
#include "amsiUtilConfig.h"

#include <iostream>

#ifdef CORE
#include <PCU.h>
#endif

MPI_Comm AMSI_COMM_WORLD = MPI_COMM_WORLD;
MPI_Comm AMSI_COMM_LOCAL = MPI_COMM_WORLD;

namespace amsi
{
  bool use_core = true;
  bool use_simmetrix = true;
  
  amsiInitializer * initializer = NULL;

  void amsiInit(int argc, char ** argv)
  {
    if(!initializer)
      std::cerr << "Please set amsi::initializer to the appropriate amsiInitializer class for the features in use." << std::endl;
    else
      initializer->amsiInit(argc,argv);
  }

  void amsiFree()
  {
    initializer->amsiFree();
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
