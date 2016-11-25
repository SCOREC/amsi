#include "amsiNonlinearAnalysis.h"
#include <cmath>
#include <iostream>
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
  bool RelativeResidualConvergence::converged()
  {
    double nrm = 0.0;
    double accm_nrm = 0.0;
    las->GetVectorNorm(nrm);
    las->GetAccumVectorNorm(accm_nrm);
    bool cnvrgd = accm_nrm == 0.0 ? false : (nrm / accm_nrm) < eps;
    std::cout << "relative residual convergence criteria: " << std::endl
              << "\t" << nrm << " / " << accm_nrm << " < " << eps << std::endl
              << "\t" << (accm_nrm == 0.0 ? 0.0 : nrm / accm_nrm) << " < " << eps << std::endl
              << "\t" << (cnvrgd ? "TRUE" : "FALSE") << std::endl;
    return cnvrgd;
  }
  bool RelativeResidualConvergence::failed()
  {
    return false;
  }
  bool IncrementalResidualConvergence::converged()
  {
    double nrm_i  = 0.0;
    double nrm = 0.0;
    las->GetVectorNorm(nrm_i);
    las->GetAccumVectorNorm(nrm);
    bool cnvrgd = nrm == 0.0 ? false : std::abs(nrm_i - nrm_im) / nrm < eps;
    return cnvrgd;
  }
  bool IncrementalResidualConvergence::failed()
  {
    return false;
  }
}
