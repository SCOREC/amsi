#ifndef AMSI_NONLINEAR_ANALYSIS_H_
#define AMSI_NONLINEAR_ANALYSIS_H_
#include <iostream>
#include <limits>
#include <vector>
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
  struct PerIter
  {
    virtual void iter() = 0;
  };
  struct PerStep
  {
    virtual void step() = 0;
  };
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
    std::vector<PerIter*> xtras;
  public:
    Iteration() : itr(0) {}
    virtual void iterate()
    {
      for(auto xtra = xtras.begin(); xtra != xtras.end(); ++xtra)
        (*xtra)->iter();
      ++itr;
    }
    int iteration() { return itr; }
    void addExtra(PerIter * pi) { xtras.push_back(pi); }
    void reset() { itr = 0; }
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
    virtual ~Convergence() {};
  };
  /**
   * The updating convergence class allows the internal
   *  epsilon value to change depending on the state of
   *  the simulation. The eps_gen template member object
   *  must have an operator() member function that returns
   *  the current epsilon value to use for detecting
   *  convergence.
   */
  template <typename V, typename E, typename R>
    class UpdatingConvergence : public Convergence
  {
  protected:
    Iteration * itr;
    double cvg_vl;
    double eps;
    double ref_vl;
    V cvg_gen;
    E eps_gen;
    R ref_gen;
  public:
    UpdatingConvergence(Iteration * it, V v, E e, R r)
      : itr(it)
      , cvg_vl(0.0)
      , eps(1e-16)
      , ref_vl(std::numeric_limits<double>::max())
      , cvg_gen(v)
      , eps_gen(e)
      , ref_gen(r)
    { }
    ~UpdatingConvergence()
    {
      delete cvg_gen;
      delete eps_gen;
      delete ref_gen;
    }
    virtual void update()
    {
      cvg_vl = (*cvg_gen)();
      eps = (*eps_gen)(itr->iteration());
      ref_vl = (*ref_gen)();
    }
    virtual bool converged()
    {
      update();
      bool cvrgd = false;
      std::cout << "convergence criteria: " << std::endl
                << "\t" << cvg_vl << " < " << eps << " * " << ref_vl << std::endl
                << "\t" << cvg_vl << " < " << eps * ref_vl << std::endl
                << "\t" << ((cvrgd = cvg_vl < eps * ref_vl) ? "TRUE" : "FALSE") << std::endl;
      return cvrgd;
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
    std::vector<Convergence*> cvgs;
  public:
    template <typename I>
    MultiConvergence(I bgn, I end)
      : Convergence()
      , cvgs()
    {
      std::copy(bgn,end,std::back_inserter(cvgs));
    }
    virtual bool converged()
    {
      for(auto cvg = cvgs.begin(); cvg != cvgs.end(); ++cvg)
        if(!(*cvg)->converged())
          return false;
      return true;
    }
    virtual bool failed()
    {
      for(auto cvg = cvgs.begin(); cvg != cvgs.end(); ++cvg)
        if((*cvg)->failed())
          return true;
      return false;
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
}
#include "amsiNonlinearAnalysis_impl.h"
#endif
