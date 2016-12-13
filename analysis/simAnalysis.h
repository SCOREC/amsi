#ifndef AMSI_SIM_ANALYSIS_H_
#define AMSI_SIM_ANALYSIS_H_
#include "sim.h"
namespace amsi
{
  /**
   * Get the next analysis case to run. If cs_nm is the empty string,
   *  the next unused analysis case is returned, otherwise an analysis
   *  with name cs_nm is returned. If all analyses have been run, or
   *  the specified analysis case doesn't exist, returns NULL.
   */
  pACase getNextAnalysisCase(pGModel mdl, const std::string & cs_nm);
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
