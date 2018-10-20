#include "amsiNonlinearAnalysis.h"
namespace amsi
{
  LinearConvergence linear_convergence;
  bool numericalSolve(Iteration * it, Convergence * cn)
  {
    do
    {
      if (cn->failed())
      {
        return false;
      }
      it->iterate();
      if (it->failed())
      {
        return false;
      }
    } while (!cn->converged());
    it->reset();
    return true;
  }
}  // namespace amsi
