#ifndef FEASOLVERS_H_
#define FEASOLVERS_H_
#include "FEA.h"
#include "LAS.h"
#include "amsiNonlinearAnalysis.h"
#include <string>
namespace amsi {
  namespace Analysis {
    class FEAIteration : public Iteration
    {
    private:
      FEA * fea;
      LAS * las;
      int iter;
    public:
      FEAIteration(FEA * f,LAS * l)
        : fea(f)
        , las(l)
        , iter(0)
      { }
      void iterate()
      {
        int gdofs, ldofs, ldof;
        fea->ApplyBC_Dirichlet();
        fea->RenumberDOFs();
        fea->GetDOFInfo(gdofs,ldofs,ldof);
        las->Reinitialize(ldofs,gdofs,ldof);
        las->Zero();
        fea->Assemble(las);
        las->Solve();
        double * sl = NULL;
        las->GetSolution(sl);
        fea->UpdateDOFs(sl);
        iter++;
      }
    };
    void LinearSolver(FEA*,LAS*);
    void NewtonSolver(FEA*,LAS*,int,double,double,double&);

  }
}
#endif
