#include "amsiAnalysis.h"
namespace amsi
{
  void reinitializeLinearSystem(Analysis * nlss, LAS * ls)
  {
    long gbl = 0;
    long lcl = 0;
    long ffst = 0;
    nlss->getDOFInfo(gbl,lcl,ffst);
    ls->Reinitialize(lcl,gbl,ffst);
  }
}
