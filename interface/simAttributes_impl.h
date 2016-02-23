#include "simWrapper.h"
namespace amsi
{
  template <class O>
    void getCasesInfoType(pAManager attm, O out)
  {
    std::vector<pACase> css;
    cutPaste<pACase>(AMAN_cases(attm),std::back_inserter(css));
    for(auto cs : css)
      *out++ = AttNode_infoType(cs);
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
}
