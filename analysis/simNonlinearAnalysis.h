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
    pAttInfoDouble eps;
  public:
    SimUpdatingEpsilon(pAttInfoDouble e)
      : eps(e)
    { }
    double operator()(double t)
    {
      if(AttInfoDouble_isExpression(eps))
        AttNode_setTime((pANode)eps,t);
      return AttInfoDouble_value(eps);
    }
  };
}
#endif
