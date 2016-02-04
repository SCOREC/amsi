#ifndef SIM_ATTRIBUTES_H_
#define SIM_ATTRIBUTES_H_
#include "sim.h"
#include "SimFEA.h"
#include <apfDynamicArray.h>
#include <list>
// decleration of simmetrix api function
pAManager SModel_attManager(pModel model);
namespace amsi
{
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
   */
  void getWithAttribute(pGModel mdl,
                        const char * attr,
                        std::list<pGEntity> & ents);
  /**
   * applyAttributes(SimFEA*);
   */
  void applyAttributes(Analysis::SimFEA* fea);
}
#endif
