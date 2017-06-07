#ifndef SIM_NONLINEAR_ANALYSIS_H_
#define SIM_NONLINEAR_ANALYSIS_H_
#include "amsiNonlinearAnalysis.h"
#include "amsiOperators.h"
#include "sim.h"
#include <apf.h>
namespace amsi
{
  class LAS;
  Convergence * buildSimConvergenceOperator(pACase cs,
                                            pAttribute at,
                                            Iteration * it,
                                            LAS * las);
  // : public
  class SimUpdatingEpsilon : public R1_to_R1
  {
  protected:
    pAttributeDouble eps;
    int cap;
  public:
    SimUpdatingEpsilon(pAttributeDouble e)
      : eps(e)
      , cap(-1)
    { }
    double operator()(double t)
    {
      if(cap != -1 && t >= cap)
        return true;
      if(!AttributeDouble_constant(eps))
        return AttributeDouble_evalDT(eps,t);
      else
        return AttributeDouble_value(eps);
    }
    void setCap(int c)
    {
      cap = c;
    }
  };
}
#endif
