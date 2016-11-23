#ifndef AMSI_SIM_ANALYSIS_H_
#define AMSI_SIM_ANALYSIS_H_
#include "sim.h"
namespace amsi
{
  /**
   * Get all mesh entities with dimensionality dm classified on the specified geometric entity.
   */
  template <typename O>
    void getClassifiedEnts(pMesh msh, pGEntity mdl_ent, int dm, O out);
  /**
   * Get all mesh entities classified on the model entities from dimension ld to dimension hd inclusive [ld,hd]
   */
  template <typename O>
    void getClassifiedDimEnts(pMesh msh, pGEntity mdl_ent, int ld, int hd, O out);
}
#include "simAnalysis_impl.h"
#endif
