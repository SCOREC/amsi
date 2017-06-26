#ifndef AMSI_LAS2_H_
#define AMSI_LAS2_H_
namespace las
{
  class Mat;
  class Vec;
  class LasOps
  {
  public:
    virtual void zero(Mat * m) = 0;
    virtual void zero(Vec * v) = 0;
    virtual void assemble(Vec * v, int cnt, int * rws, double * vls) = 0;
    virtual void assemble(Mat * m, int cntr, int * rws, int cntc, int * cls, double * vls) = 0;
    virtual void set(Vec * v, int cnt, int * rws, double * vls) = 0;
    virtual void set(Mat * m, int cntr, int * rws, int cntc, int * cls, double * vls) = 0;
    virtual double norm(Vec * v) = 0;
    virtual double dot(Vec * v0, Vec * v1) = 0;
    virtual void axpy(double a, Vec * x, Vec * y) = 0;
    virtual void get(Vec * v, double *& vls) = 0;
    virtual void restore(Vec * v, double *& vls) = 0;
  };
  class LasSolve
  {
    virtual void solve(Mat * k, Vec * u, Vec * f) = 0;
  };
}
#endif
