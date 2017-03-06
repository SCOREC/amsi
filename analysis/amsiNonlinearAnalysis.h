#ifndef AMSI_NONLINEAR_ANALYSIS_H_
#define AMSI_NONLINEAR_ANALYSIS_H_
#include "amsiLAS.h"
namespace amsi
{
  class Iteration;
  class Convergence;
  /**
   * Perform a numerical solve using the supplied iteration
   *  and convergence objects. The iteration object
   *  constitutes a single iteration in a nonlinear solve.
   *  The convergence object informs whether the underlying
   *  simulation has converged to a solution for the current
   *  nonlinear solve.
   *  If the simulation is linear, simply pass a convergence
   *  object that returns true and an iteration object that
   *  performs the linear solve.
   */
  bool numericalSolve(Iteration * it, Convergence * cn);
  /**
   * An iteration object that represents a single iteration
   *  for a numerical simulation. The base class merely tracks
   *  the iteration count as a single linear solve takes place.
   *  Classes which inherit from the Iteration should form the
   *  linear system for a simulation and perform a solve of
   *  of the linear system. A suitable convergence operator
   *  to detect whether the state of the simulation after
   *  the linear system constitutes a solved state.
   */
  class Iteration
  {
    int itr;
  public:
    Iteration() : itr(0) {}
    virtual void iterate() { ++itr; }
    int iteration() { return itr; }
};
  /**
   * A convergence operator determines whether the current
   *  state of the simulation has converged to a reasonable
   *  solution.
   */
  class Convergence
  {
  public:
    virtual bool converged() = 0;
    virtual bool failed() {return false;}
  };
  /**
   * The updating convergence class allows the internal
   *  epsilon value to change depending on the state of
   *  the simulation. The eps_gen template member object
   *  must have an operator() member function that returns
   *  the current epsilon value to use for detecting
   *  convergence.
   */
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
  /**
   * A convergence class that wraps two convergence
   *  objects. Useful for composing simple convergence
   *  operations in lieu of implementing a bespoke
   *  combined class.
   */
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
  /**
   * A convergence class to use for linear
   *  simulations, since no iteration is required
   *  for linear simulations, this simply returns
   *  true.
   */
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
