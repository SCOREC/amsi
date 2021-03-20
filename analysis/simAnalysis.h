#ifndef AMSI_SIM_ANALYSIS_H_
#define AMSI_SIM_ANALYSIS_H_
#include "sim.h"
#include <string>
namespace amsi
{
  /**
   * Get a pointer to the analysis case with the specified name
   * \param [in] mdl simmetrix model
   * \return pointer to analysis cases
   */
  pACase getAnalysisCase(pGModel mdl, std::string const & case_name);
  /**
   * Get all mesh entities with dimensionality dm classified on the specified geometric entity.
   */
  template <typename O>
    void getClassifiedEnts(pMesh msh, pGEntity mdl_ent, int dm, O out);
}
#include "simAnalysis_impl.h"
#endif
