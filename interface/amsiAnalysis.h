#ifndef AMSI_ANALYSIS_H_
#define AMSI_ANALYSIS_H_
#include "amsiLAS.h"
#include "amsiFEA.h"
#include "amsiNonlinearAnalysis.h"
#include <list>
namespace amsi
{
  class FEAIteration : public Iteration
  {
  protected:
    FEA * pde;
    std::list<apf::Field *> flds;
    //std::list<apf::Numbering *> sl_flds;
    pMesh prt;
    std::list<BCQuery*> dir_bcs;
    std::list<BCQuery*> nue_bcs;
    LAS * ls;
    int itr;
  public:
    FEAIteration(FEA * f, LAS * l)
      : pde(f)
      , ls(l)
      , itr(0)
    { }
    virtual void iterate()
    {
      //reinitializeLinearSystem(pde,ls);
      //pde->applyNeumannBCs(ls);
      //pde->assembleLinearSystem(ls);
      ls->solve();
      //pde->applySolution(ls);
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
