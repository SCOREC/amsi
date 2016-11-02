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
    LAS * las;
    pMesh simmsh;
//    std::vector<SimBCQuery*> bcs[amsi::NUM_BC_TYPES];
    double t;
    int itr;
  public:
    FEAIteration()
    { }
    virtual void iterate()
    {
/*
      applyAllSimDirichletBCs(nms.begin(),
                              nms.end(),
                              simmsh,
                              bcs[amsi::DIRICHLET].begin(),
                              bcs[amsi::DIRICHLET].end(),
                              t);
*/
      //pde->applyNeumannBCs(ls);
      //pde->assembleLinearSystem(ls);
      las->solve();
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
