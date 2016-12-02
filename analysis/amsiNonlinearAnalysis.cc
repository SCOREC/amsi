#include "amsiNonlinearAnalysis.h"
namespace amsi
{
  bool numericalSolve(Iteration * it, Convergence * cn)
  {
    bool scs = true;
    do
    {
      it->iterate();
      if(cn->failed())
      {
        scs = false;
        break;
      }
    } while(!cn->converged());
    return scs;
  }
}
