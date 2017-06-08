#include "simAnalysis.h"
#include "simAttributes.h"
#include <vector>
namespace amsi
{
  std::vector<pACase> getAnalysisCases(pGModel mdl) {
    std::vector<pACase> cases;
    amsi::getTypeCases(SModel_attManager(mdl),"analysis",std::back_inserter(cases));
    return cases;
  }
  pACase getAnalysisCase(pGModel mdl, std::string const & case_name) {
    return AMAN_findCase(SModel_attManager(mdl), case_name.c_str());
  }
}
