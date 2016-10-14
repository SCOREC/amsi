#ifndef AMSI_NONLINEAR_ANALYSIS_H_
#define AMSI_NONLINEAR_ANALYSIS_H_
#include "amsiLAS.h"
namespace amsi
{
  class Iteration;
  class Convergence;
  bool numericalSolve(Iteration * it, Convergence * cn);
  class Iteration
  {
  public:
    virtual void iterate() = 0;
  };
  class Convergence
  {
  public:
    virtual bool converged() = 0;
    virtual bool failed() {return false;}
  };
  class LinearConvergence : public Convergence
  {
  public:
    virtual bool converged() {return true;}
  };
  class RelativeResidualConvergence : public Convergence
  {
  protected:
    LAS * las;
    double eps;
  public:
    RelativeResidualConvergence(LAS * l, double e)
      : las(l)
      , eps(e)
    { }
    bool converged();
    bool failed();
  };
  class IncrementalResidualConvergence : public Convergence
  {
  protected:
    LAS * las;
    double eps;
    double nrm_im;
  public:
    IncrementalResidualConvergence(LAS * l, double e)
      : las(l)
      , eps(e)
      , nrm_im(0.0)
    { }
    bool converged();
    bool failed();
  };
}
#endif
