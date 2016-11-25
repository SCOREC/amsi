#ifndef AMSI_SIM_ATTRIBUTES_H_
#define AMSI_SIM_ATTRIBUTES_H_
#include "sim.h"
#include <apfDynamicArray.h>
#include <list>
#include <vector>
// external decleration of simmetrix api function for visibility
pAManager SModel_attManager(pModel model);
namespace amsi
{
  bool isAttrConst(pAttribute att);
  enum SimCaseAttributes
  {
    PROBLEM_DEFINITION = 0,
    SOLUTION_STRATEGY = 1,
    OUTPUT = 2,
    NUM_SIM_CASE_ATTRS = 3
  };
  const char * getSimCaseAttributeDesc(int tp);
  void initCase(pModel mdl, pACase cs);
  void freeCase(pACase cs);
  const char * attRepTypeString(AttRepType tp);
  void describeNode(pANode nd);
  void describeAttribute(pAttribute at);
  /**
   * Associate an attribute case given by attr_cs with the given model.
   */
  void initAttributeCase(pGModel mdl, const char * att_cs);
  /**
   * Unassociate the current attribute case
   */
  void clearModelAttributes(pGModel mdl);
  /**
   * Determine if an attribute is an expression that needs to be evaluated
   */
  bool requiresIncrementalLoading(pGModel mdl, const char * attr);
  /**
   * Determine if a geometric entity has the specified attribute
   */
  bool hasAttribute(pGEntity entity, const char * attr);
  /**
   * Get all entities with the specified attribute
   * @note only here for legacy purposes
   * @todo should be refactored into iterator version when convenient
   */
  void getWithAttribute(pGModel mdl,
                        const char * attr,
                        std::list<pGEntity> & ents);
  void writeAttMan(pAManager attmn, const char * fnm);
  /**
   * Get the infotype of all attribute cases in the attribute manager
   * @note this will leak memory unless Sim_deleteString is called on each returned c-string
   */
  template <class O>
    void getCasesInfoType(pAManager attm, O out);
  /**
   * Get all cases managed by the attribute manager with infotype tp
   */
  template <class O>
    void getTypeCases(pAManager attm, const char * tp, O out);
  template <class O>
    void getTypeNodes(pANode nd, const char * tp, O out);
  template <class O>
    void getTrackedModelItems(pACase cs, const char * tp, O out);
}
#include "simAttributes_impl.h"
#endif
