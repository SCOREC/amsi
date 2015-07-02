#ifndef NONLINELASTICITY_H_
#define NONLINELASTICITY_H_

#include <apfSimFEA.h>

namespace amsi
{
  namespace Analysis
  {
    class NonLinElasticity : public apfSimFEA
    {
    private:
      double poisson_ratio;
      double youngs_modulus;
      
    public:
      NonLinElasticity(MPI_Comm comm,
		       pGModel in_model,
		       pParMesh in_mesh);

      void UpdateSolution(const double *);
    };
    
  }
}

#endif
