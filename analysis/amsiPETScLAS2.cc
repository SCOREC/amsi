#include "amsiPETScLAS2.h"
namespace petsc
{
#include <petsc.h>
}
namespace las
{
  // todo : create variant using nnz structure
  Mat * createPetscMatrix(int g, int l)
  {
    petsc::Mat * m = new petsc::Mat;
    petsc::MatCreateAIJ(PETSC_COMM_WORLD,
                        l,l,g,g,
                        sqrt(g), // dnz approximation
                        PETSC_NULL,
                        sqrt(g), // onz approximation
                        PETSC_NULL,
                        m);
    petsc::MatSetOption(*m,MAT_NEW_NONZERO_ALLOCATION_ERROR,PETSC_FALSE);
    return reinterpret_cast<Mat*>(m);
  }
  Vec * createPetscVector(int g, int l)
  {
    petsc::Vec * v = new petsc::Vec;
    petsc::VecCreateMPI(PETSC_COMM_WORLD,l,g,v);
    petsc::VecSetOption(*v,VEC_IGNORE_NEGATIVE_INDICES,PETSC_TRUE);
    return reinterpret_cast<Vec*>(v);
  }
  petsc::Mat * getPetscMat(Mat * m)
  {
    return reinterpret_cast<petsc::Mat*>(m);
  }
  petsc::Vec * getPetscVec(Vec * v)
  {
    return reinterpret_cast<petsc::Vec*>(v);
  }
  LasOps * initPetscOps()
  {
    return new PetscOps;
  }
  void PetscOps::zero(Mat * m)
  {
    petsc::MatZeroEntries(*getPetscMat(m));
  }
  void PetscOps::zero(Vec * v)
  {
    petsc::VecZeroEntries(*getPetscVec(v));
  }
  void PetscOps::add(Vec * v, int cnt, int * rws, double * vls)
  {
    petsc::VecSetValues(*getPetscVec(v),cnt,rws,vls,ADD_VALUES);
  }
  void PetscOps::add(Mat * m, int cntr, int * rws, int cntc, int * cls, double * vls)
  {
    petsc::MatSetValues(*getPetscMat(m),cntr,rws,cntc,cls,vls,ADD_VALUES);
  }
  void PetscOps::solve(Mat * k, Vec * u, Vec * f)
  {
    petsc::Mat * pk = getPetscMat(k);
    petsc::Vec * pu = getPetscVec(u);
    petsc::Vec * pf = getPetscVec(f);
    petsc::Solver s;
    petsc::KSPCreate(PETSC_COMM_WORLD,&s);
    petsc::VecAssemblyBegin(*pf);
    petsc::VecAssemblyEnd(*pf);
    petsc::MatAssemblyBegin(*pk,MAT_FINAL_ASSEMBLY);
    petsc::MatAssemblyEnd(*pk,MAT_FINAL_ASSEMBLY);
    petsc::KSPSetOperators(s,pk,pk);
    petsc::KSPSetFromOptions(s);
    petsc::KSPSolve(s,f,u);
  }
}
