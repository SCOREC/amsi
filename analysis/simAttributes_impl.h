#ifndef SIM_ATTRIBUTES_IMPL_H_
#define SIM_ATTRIBUTES_IMPL_H_
#include "simWrapper.h"
#include "simBoundaryConditions.h"
#include <cassert>
namespace amsi
{
  template <class O>
    void getCasesInfoType(pAManager attm, O out)
  {
    std::vector<pACase> css;
    cutPaste<pACase>(AMAN_cases(attm),std::back_inserter(css));
    auto nd = css.end();
    for(auto cs = css.begin(); cs != nd; ++cs)
      *out++ = AttNode_infoType(*cs);
  }
  template <class O>
    void getTypeCases(pAManager attm, const char * tp, O out)
  {
    cutPaste<pACase>(AMAN_findCasesByType(attm,tp),out);
  }
  template <class O>
    void getTypeNodes(pANode nd, const char * tp, O out)
  {
    cutPaste<pANode>(AttNode_childrenByType(nd,tp),out);
  }
  template <class O>
    void getTrackedModelItems(pACase cs, const char * tp, O out)
  {
    pACase op = (pACase)AttNode_childByType((pANode)cs,"output");
    pANode tp_op = AttNode_childByType((pANode)op,tp);
    getAssociatedModelItems(op,tp_op,out);
  }
}
#endif
