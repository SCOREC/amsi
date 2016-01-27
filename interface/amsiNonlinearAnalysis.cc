#include "amsiNonlinearAnalysis.h"
namespace amsi
{
  bool numericalSolve(Iteration * it, Convergence * cn)
  {
    bool failure = false;
    while(!cn->converged())
    {
      it->iterate();
      if(cn->failed())
      {
        failure = true;
        break;
      }
    }
    return failure;
  }
}
