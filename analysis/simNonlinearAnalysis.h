#ifndef SIM_NONLINEAR_ANALYSIS_H_
#define SIM_NONLINEAR_ANALYSIS_H_
#include "amsiNonlinearAnalysis.h"
#include "amsiOperators.h"
#include "sim.h"
#include <apf.h>
namespace amsi
{
  Convergence * buildSimConvergenceOperator(pACase cs,
                                            pANode nd,
                                            Iteration * it,
                                            LAS * las);
  // : public
  class SimUpdatingEpsilon : public R1_to_R1
  {
  protected:
    pAttributeDouble eps;
  public:
    SimUpdatingEpsilon(pAttributeDouble e)
      : eps(e)
    { }
    double operator()(double t)
    {
      if(AttributeDouble_constant(eps) != 1)
        return AttributeDouble_evalDT(eps,t);
      return AttributeDouble_value(eps);
    }
  };
}
#endif
