#include "amsiFEA.h"
#include <algorithm>
#include <cassert>
#include <fstream>
#include <vector>
namespace amsi
{
  FEA::FEA(MPI_Comm cm)
    : local_dof_count(0)
    , first_local_dof(0)
    , global_dof_count(0)
    , fixed_dofs(0)
    , numbered(false)
    , simulation_time(0.0)
    , analysis_comm(cm)
    , dir_bcs()
    , neu_bcs()
  { }

}
