#ifndef AMSI_ANALYSIS_H_
#define AMSI_ANALYSIS_H_
#include "amsiLAS.h"
#include "amsiFEA.h"
#include "amsiNonlinearAnalysis.h"
#include <list>
namespace amsi
{
  class DiscreteAnalysis
  {
  public:
    virtual void applyDirichletBCs() = 0;
    virtual void applyNeumannBCs(LAS * ls) = 0;
    virtual void assembleLinearSystem(LAS * ls) = 0;
    virtual void applySolution(LAS * ls) = 0;
    virtual void calculateDOFInfo(long & gbl, long & lcl, long & ffst) = 0;
  };
  /*
  class FiniteElementAnalysis : public DiscreteAnalysis
  {
  protected:
    std::list<BCQuery*> dir_bcs;
    std::list<BCQuery*> neu_bcs;
  public:
    virtual void applyDirichletBCs();
    virtual void applyNeumannBCs(LAS * ls);
    virtual void assembleLinearSystem(LAS * ls);
    virtual void applySolution(LAS * ls);
    virtual void calculateDOFInfo(long & gbl, long & lcl, long & ffst);
  };
  */
  //void reinitializeLinearSystem(DiscreteAnalaysis * nlss, LAS * ls);
  class DiscreteAnalysisIteration : public Iteration
  {
  protected:
    DiscreteAnalysis * pde;
    LAS * ls;
    int itr;
  public:
    DiscreteAnalysisIteration(DiscreteAnalysis * f, LAS * l)
      : pde(f)
      , ls(l)
      , itr(0)
    { }
    virtual void iterate()
    {
      pde->applyDirichletBCs();
      //reinitializeLinearSystem(pde,ls);
      pde->applyNeumannBCs(ls);
      pde->assembleLinearSystem(ls);
      ls->solve();
      pde->applySolution(ls);
      itr++;
    }
  };
  class NewtonRaphsonConvergence : public Convergence
  {
  public:
    NewtonRaphsonConvergence(LAS * las);
  };
}
#endif
