#ifndef NONLINELASTICITY_H_
#define NONLINELASTICITY_H_
#include <apfSimFEA.h>
namespace amsi {
  class NonLinElasticity : public apfSimFEA {
    private:
    double poisson_ratio;
    double youngs_modulus;

    public:
    NonLinElasticity(pGModel in_model, pParMesh in_mesh, pACase pd, pACase ss,
                     MPI_Comm cm = AMSI_COMM_SCALE);
  };
}  // namespace amsi
#endif
