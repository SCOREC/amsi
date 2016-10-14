#ifndef PETSCITERATIVESOLVER_H_
#define PETSCITERATIVESOLVER_H_
#include "amsiLAS.h"
#include <petscksp.h>
#include <petscmat.h>
#include <vector>
namespace amsi
{
  class PetscLAS : public LAS
  {
  public:
    PetscLAS();
    PetscLAS(int,int);
    void iter();
    void step();
    void Reinitialize(int, int, int, int*);
    void Reinitialize(int, int, int);
    void AddToMatrix(int,int,double);
    void AddToMatrix(int,int*,int,int*,double*);
    void AddToVector(int,double);
    void AddToVector(int,int*,double*);
    void solve();
    bool Zero();
    bool ZeroMatrix();
    bool ZeroVector();
    void GetVector(double *&);
    void SetVector(const double *);
    void GetVectorNorm(double &);
    void GetAccumVector(double *&);
    void GetAccumVectorNorm(double &);
    void GetDotNorm(double &);
    void GetSolution(double *&);
    void GetSolutionNorm(double & n);
    void GetAccumSolutionNorm(double &);
    virtual void PrintMatrix(std::ostream &);
    virtual void PrintVector(std::ostream &);
    virtual void PrintSolution(std::ostream &);
    double MatrixMax();
    virtual int GlobalDOFs() { return globalNumEqs; }
    virtual int LocalDOFs() { return vec_high - vec_low; }
    virtual int LocalOffset() { return vec_low; }
    ~PetscLAS();
  private:
    void freeMem();
    Mat A;     // matrix
    Vec x_im;   // previous solution
    Vec x_i;     // current solution
    Vec x;
    Vec b_im;   // previous vector
    Vec b_i;     // current vector
    Vec b;
    Vec w;     // work vector
    double * x_arr, * b_arr, * b_i_arr;
    int globalNumEqs;
    int vec_low;
    int vec_high;
    int mat_low;
    int mat_high;
    bool b_assembled, b_addMode;
    KSP solver;
  };
}
#endif
