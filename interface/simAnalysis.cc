#include "simAnalysis.h"
namespace amsi
{
  SimFEA * buildSimFEA(const char * msh_nm, SimAnalysis * an)
  {
    pParMesh msh = PM_load(msh_nm, sthreadNone, an->mdl, NULL);
    return new SimFEA(msh,an);
  }
}
