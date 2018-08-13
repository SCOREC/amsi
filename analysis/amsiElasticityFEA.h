#ifndef AMSI_ELASTICITY_FEA_H_
#define AMSI_ELASTICITY_FEA_H_
#include <apf.h>
#include <apfNumbering.h>
#include <apfSIM.h>
#include <apfShape.h>
#include "apfSimFEA.h"
namespace amsi {
  class ElasticityFEA : public apfSimFEA {
    protected:
    double youngs_modulus;
    double poisson_ratio;

    public:
    ElasticityFEA(pGModel in_model, pParMesh in_mesh, pACase pd, pACase ss,
                  MPI_Comm cm = AMSI_COMM_SCALE, double E = 20000,
                  double v = 0.3);
  };
}  // namespace amsi
#endif
