#include "FEA.h"
#include <algorithm>
#include <cassert>
#include <fstream>
#include <vector>
namespace amsi {
  namespace Analysis {
    FEA::FEA(MPI_Comm comm, const std::string & in_name)
      : local_dof_count(0)
      , first_local_dof(0)
      , global_dof_count(0)
      , fixed_dofs(0)
      , numbered(false)
      , name(in_name)
      , analysis_comm(comm)
    { }
  }
}
