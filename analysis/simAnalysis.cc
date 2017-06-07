#include "simAnalysis.h"
#include "simAttributes.h"
namespace amsi
{
  pACase getNextAnalysisCase(pGModel mdl, const std::string & cs_nm)
  {
    static std::vector<pACase> css;
    static bool init = false;
    pACase rslt = NULL;
    if(!init)
    {
      amsi::getTypeCases(SModel_attManager(mdl),"analysis",std::back_inserter(css));
      init = true;
    }
    if(cs_nm.empty())
    {
      rslt = css.back();
      css.pop_back();
    }
    else
      for(auto cs = css.begin(); cs != css.end(); ++cs)
      {
        char * nm = AttNode_name(*cs);
        if(cs_nm.compare(nm) == 0)
        {
          rslt = *cs;
          css.erase(cs);
          break;
        }
        Sim_deleteString(nm);
      }
    return rslt;
  }
}
