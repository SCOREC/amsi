/******************************************************************************

  (c) 2015 Scientific Computation Research Center,
      Rensselaer Polytechnic Institute. All rights reserved.

  The LICENSE-SCOREC file included with this distribution describes the terms
  of the SCOREC Non-Commercial License this program is distributed under.

*******************************************************************************/

#include "FEA.h"

// Standard libs
#include <algorithm>
#include <cassert>
#include <fstream>
#include <vector>

namespace amsi {
  namespace Analysis {

    FEA::FEA(MPI_Comm comm, const std::string & in_name) :
      local_dof_count(0),
      first_local_dof(0),
      global_dof_count(0),
      fixed_dofs(0),
      numbered(false),
      name(in_name),
      analysis_comm(comm)
    { }

    std::ostream& operator<<(std::ostream& out, const FEA & analysis)
    {
      out << analysis.GetName();
      return out;
    }

  }
} // end of namespace SCOREC::Core
