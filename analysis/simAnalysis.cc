#include "simAnalysis.h"
#include "simAttributes.h"
#include <vector>
namespace amsi
{
  pACase getAnalysisCase(pGModel mdl, std::string const & case_name)
  {
    return AMAN_findCase(SModel_attManager(mdl), case_name.c_str());
  }
}
