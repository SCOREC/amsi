#ifndef SIM_NONLINEAR_ANALYSIS_H_
#define SIM_NONLINEAR_ANALYSIS_H_
#include "amsiNonlinearAnalysis.h"
#include "amsiOperators.h"
#include <apf.h>
#include <model_traits/AssociatedModelTraits.h>
namespace amsi
{
  class LAS;
  std::unique_ptr<Convergence> createConvergenceOperator(const mt::AssociatedCategoryNode& nd,
                                            MultiIteration * it,
                                            LAS * las);
}
#endif
