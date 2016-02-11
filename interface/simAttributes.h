#ifndef SIM_ATTRIBUTES_H_
#define SIM_ATTRIBUTES_H_
#include "sim.h"
#include "SimFEA.h"
#include "simWrapper.h"
#include <apfDynamicArray.h>
#include <vector>
// external decleration of simmetrix api function for visibility
pAManager SModel_attManager(pModel model);
namespace amsi
{
  extern char const * sim_attr_css[];
  void initCase(pModel mdl, pACase cs);
  void freeCase(pACase cs);
  const char * attRepTypeString(AttRepType tp);
  void describeNode(pANode nd);
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
  void writeAttMan(pAManager attmn, const char * fnm);
  /**
   * Get the infotype of all attribute cases in the attribute manager
   */
  template <class O>
    void getCasesInfoType(pAManager attm, O out)
  {
    std::vector<pACase> css;
    cutPaste<pACase>(AMAN_cases(attm),std::back_inserter(css));
    for(auto cs : css)
      *out++ = AttNode_infoType(cs);
    /*
    pPList lst = AMAN_cases(attm);
    auto nd = end<pACase>(lst);
    for(auto cs = begin<pACase>(lst); cs != nd; ++cs)
      *output++ = AttNode_infoType(*cs);
    PList_delete(lst);
    */
  }
  /**
   * Get all cases managed by the attribute manager with infotype tp
   */
  template <class O>
    void getTypeCases(pAManager attm, const char * tp, O out)
  {
    cutPaste<pACase>(AMAN_findCasesByType(attm,tp),out);
    /*
    pPList lst = AMAN_findCasesByType(attm,tp);
    std::copy(begin<pACase>(lst), end<pACase>(lst), out);
    PList_delete(lst);
    */
  }
  template <class O>
    void getTypeNodes(pANode nd, const char * tp, O out)
  {
    cutPaste<pANode>(AttNode_childrenByType(nd,tp),out);
    /*
    pPList lst = AttNode_childrenByType(nd,tp);
    std::copy(begin<pANode>(lst), end<pANode>(lst), out);
    PList_delete(lst);
    */
  }
}
#endif
