#ifndef AMSI_PETSC_LAS2_H_
#define AMSI_PETSC_LAS2_H_
#include "amsiLAS2.h"
namespace las
{
  Mat * createPetscMatrix(int gbl, int lcl);
  Vec * createPetscVector(int gbl, int lcl);
  class PetscOPs : public LasOps
  {
  public:
    virtual void zero(Mat * m);
    virtual void zero(Vec * v);
    virtual void add(Vec * v, int cnt, int * rws, double * vls);
    virtual void add(Mat * m, int cntr, int * rws, int cntc, int * cls, double * vls);
    virtual void solve(Mat * k, Vec * u, Vec * f);
    virtual void dot(Vec * v0, Vec * v1);
    virtual void norm(Vec * v);
  };
}
#endif
