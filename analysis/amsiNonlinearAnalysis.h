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
    int itr;
  public:
    Iteration() : itr(0) {}
    virtual void iterate() { ++itr; }
    int iteration() { return itr; }
  };
  class Convergence
  {
  public:
    virtual bool converged() = 0;
    virtual bool failed() {return false;}
  };
  template <typename T>
    class UpdatingConvergence : public Convergence
  {
  protected:
    double eps;
    T eps_gen;
  public:
    UpdatingConvergence(T e)
      : eps(e())
      , eps_gen(e)
    { }
    virtual void update()
    {
      eps = eps_gen();
    }
  };
  class MultiConvergence : public Convergence
  {
  private:
    Convergence * one;
    Convergence * two;
  public:
    MultiConvergence(Convergence * o, Convergence * t)
      : Convergence()
      , one(o)
      , two(t)
    {}
    virtual bool converged()
    {
      bool frst = one->converged();
      return frst ? two->converged() : frst;
    }
    virtual bool failed()
    {
      bool frst = one->failed();
      return frst ? frst : two->failed();
    }
  };
  class LinearConvergence : public Convergence
  {
  public:
    virtual bool converged() {return true;}
  };
  template <typename T>
    class RelativeResidualConvergence : public UpdatingConvergence<T>
  {
  protected:
    LAS * las;
  public:
    RelativeResidualConvergence(LAS * l, T e)
      : UpdatingConvergence<T>(e)
      , las(l)
    { }
    bool converged();
    bool failed();
  };
  template <typename T>
    class IncrementalResidualConvergence : public UpdatingConvergence<T>
  {
  protected:
    LAS * las;
    double nrm_im;
  public:
    IncrementalResidualConvergence(LAS * l, T e)
      : UpdatingConvergence<T>(e)
      , las(l)
      , nrm_im(0.0)
    { }
    bool converged();
    bool failed();
  };
}
#include "amsiNonlinearAnalysis_impl.h"
#endif
