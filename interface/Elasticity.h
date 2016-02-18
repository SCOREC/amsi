#ifndef ELASTICITY_H_
#define ELASTICITY_H_
#include "apfSimFEA.h"
#include <apf.h>
#include <apfNumbering.h>
#include <apfShape.h>
#include <apfSIM.h>
namespace amsi {
  class Elasticity : public apfSimFEA
  {
  protected:
    double youngs_modulus;
    double poisson_ratio;
  public:
    Elasticity(MPI_Comm comm,
               pGModel in_model,
               pParMesh in_mesh,
               double E = 20000, double v = 0.3);
  };
}
#endif
