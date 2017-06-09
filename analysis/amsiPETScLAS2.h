#ifndef AMSI_PETSC_LAS2_H_
#define AMSI_PETSC_LAS2_H_
#include "amsiLAS2.h"
namespace las
{
  Mat * createPetscMatrix(int gbl, int lcl);
  Vec * createPetscVector(int gbl, int lcl);
  LasOps * getPetscOps();
  class PetscOps : public LasOps
  {
  public:
    virtual void zero(Mat * m);
    virtual void zero(Vec * v);
    virtual void assemble(Vec * v, int cnt, int * rws, double * vls);
    virtual void assemble(Mat * m, int cntr, int * rws, int cntc, int * cls, double * vls);
    virtual void set(Vec * v, int cnt, int * rws, double * vls);
    virtual void set(Mat * v, int cntr, int * rws, int cntc, int * cls, double * vls);
    virtual void solve(Mat * k, Vec * u, Vec * f);
    virtual double norm(Vec * v);
    virtual double dot(Vec * v0, Vec * v1);
    virtual void axpy(double a, Vec * x, Vec * y);
    virtual void get(Vec * v, double *& vls);
    virtual void restore(Vec * v, double *& vls);
  };
}
#endif