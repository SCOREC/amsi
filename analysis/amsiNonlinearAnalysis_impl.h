#include <iostream>
#include <cmath>
namespace amsi
{
  template <typename T>
    bool RelativeResidualConvergence<T>::converged()
  {
    UpdatingConvergence<T>::update();
    double nrm = 0.0;
    double accm_nrm = 0.0;
    las->GetVectorNorm(nrm);
    las->GetAccumVectorNorm(accm_nrm);
    bool cnvrgd = accm_nrm == 0.0 ? false : (nrm / accm_nrm) < UpdatingConvergence<T>::eps;
    std::cout << "relative residual convergence criteria: " << std::endl
              << "\t" << nrm << " / " << accm_nrm << " < " << UpdatingConvergence<T>::eps << std::endl
              << "\t" << (accm_nrm == 0.0 ? 0.0 : nrm / accm_nrm) << " < " << UpdatingConvergence<T>::eps << std::endl
              << "\t" << (cnvrgd ? "TRUE" : "FALSE") << std::endl;
    return cnvrgd;
  }
  template <typename T>
    bool RelativeResidualConvergence<T>::failed()
  {
    return false;
  }
  template <typename T>
    bool IncrementalResidualConvergence<T>::converged()
  {
    UpdatingConvergence<T>::update();
    double nrm_i  = 0.0;
    double nrm = 0.0;
    las->GetVectorNorm(nrm_i);
    las->GetAccumVectorNorm(nrm);
    bool cnvrgd = nrm == 0.0 ? false : std::abs(nrm_i - nrm_im) / nrm < UpdatingConvergence<T>::eps;
    return cnvrgd;
  }
  template <typename T>
    bool IncrementalResidualConvergence<T>::failed()
  {
    return false;
  }

}
