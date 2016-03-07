#include "amsiFields.h"
namespace amsi
{
  // probably need a richer set of tools to manage the supported field types and their associated boundary conditions (ie a metamodel)
  template <typename O>
    void getApplicableBCTypesForField(int fld_tp, int bc_tp, O out)
  {
    if(fld_tp == DISPLACEMENT && bc_tp == DIRICHLET)
      *out++ = DISPLACEMENT;
    else if(fld_tp == DISPLACEMENT && bc_tp == NEUMANN)
    {
      *out++ = SURFACE_TRACTION;
      *out++ = NORMAL_PRESSURE;
    }
  }
}
