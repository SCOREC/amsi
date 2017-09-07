#ifndef AMSI_PETSC_LAS2_H_
#define AMSI_PETSC_LAS2_H_
#include "amsiPETScLAS.h"
#include "amsiLAS2.h"
#include "amsiNonlinearAnalysis.h"
#include "amsiByteStream.h" // for a hack (search for serialize)
namespace las
{
  Mat * createPetscMatrix(int gbl, int lcl);
  Vec * createPetscVector(int gbl, int lcl);
  LasOps * getPetscOps();
  LasSolve * createPetscLUSolve();
  LasSolve * createPetscQNSolve(void * a);
  class PetscOps : public LasOps
  {
  public:
    virtual void zero(Mat * m);
    virtual void zero(Vec * v);
    virtual void assemble(Vec * v, int cnt, int * rws, double * vls);
    virtual void assemble(Mat * m, int cntr, int * rws, int cntc, int * cls, double * vls);
    virtual void set(Vec * v, int cnt, int * rws, double * vls);
    virtual void set(Mat * v, int cntr, int * rws, int cntc, int * cls, double * vls);
    virtual double norm(Vec * v);
    virtual double dot(Vec * v0, Vec * v1);
    virtual void axpy(double a, Vec * x, Vec * y);
    virtual void get(Vec * v, double *& vls);
    virtual void restore(Vec * v, double *& vls);
  };
  class Petsc2Wrapper : public amsi::LAS
  {
  private:
    void * qniterargs;
    double glb_dofs;
    double lcl_dofs;
    double vec_low;
    double vec_hgh;
    LasOps * ops;
    LasSolve * slv;
    Mat * K;
    Vec * u;
    Vec * f;
    Vec * u_i;
    Vec * f_i;
    Vec * u_i1;
    Vec * f_i1;
  public:
    Petsc2Wrapper(void * a)
      : qniterargs(a)
      , glb_dofs(0)
      , lcl_dofs(0)
      , vec_low(0)
      , vec_hgh(0)
      , ops(getPetscOps())
      , slv(NULL)
      , K(NULL)
      , u(NULL)
      , f(NULL)
      , u_i(NULL)
      , f_i(NULL)
      , u_i1(NULL)
      , f_i1(NULL)
    { }
    Petsc2Wrapper(int glb,int lcl)
      : glb_dofs(glb)
      , lcl_dofs(lcl)
      , vec_low(0)
      , vec_hgh(0)
      , ops(getPetscOps())
      , slv(createPetscQNSolve(NULL))
      , K(createPetscMatrix(glb,lcl))
      , u(createPetscVector(glb,lcl))
      , f(createPetscVector(glb,lcl))
      , u_i(createPetscVector(glb,lcl))
      , f_i(createPetscVector(glb,lcl))
      , u_i1(createPetscVector(glb,lcl))
      , f_i1(createPetscVector(glb,lcl))
    {
      MPI_Scan(&lcl,&vec_hgh,1,MPI_INTEGER,MPI_SUM,PETSC_COMM_WORLD);
      vec_low = vec_hgh - lcl;
    }
    void iter()
    {
      // copy u_i into u_i1
      ops->zero(u_i1);
      ops->axpy(1.0,u_i1,u_i);
      // copy f_i into f_i1
      ops->zero(f_i1);
      ops->axpy(1.0,f_i1,f_i);
    }
    void step()
    {
      ops->zero(u_i);
      ops->zero(u_i1);
      ops->zero(f_i);
      ops->zero(u_i);
      ops->zero(u);
      ops->zero(f);
      ops->zero(K);
    }
    void Reinitialize(int lcl, int glb, int low, int *)
    {
      Reinitialize(lcl,glb,low);
    }
    void Reinitialize(int lcl, int glb, int low)
    {
      if(K == NULL)
      {
        K = createPetscMatrix(glb,lcl);
        u = createPetscVector(glb,lcl);
        f = createPetscVector(glb,lcl);
        u_i = createPetscVector(glb,lcl);
        f_i = createPetscVector(glb,lcl);
        u_i1 = createPetscVector(glb,lcl);
        f_i1 = createPetscVector(glb,lcl);
        lcl_dofs = lcl;
        glb_dofs = glb;
        MPI_Scan(&lcl,&vec_hgh,1,MPI_INTEGER,MPI_SUM,PETSC_COMM_WORLD);
        vec_low = vec_hgh - lcl;
        las::Vec ** ff = &f;
        qniterargs = serialize(qniterargs,ff);
        slv = createPetscQNSolve(qniterargs);
      }
      else if(lcl != lcl_dofs || glb != glb_dofs || low != vec_low)
      {
        std::cerr << "ERROR! Linear system resizing not supported for Petsc2Wrapper class!" << std::endl;
        MPI_Abort(PETSC_COMM_WORLD,-1);
      }
    }
    void AddToMatrix(int,int,double) {}
    void AddToMatrix(int cr,int * r,int cc,int * c,double * v)
    {
      ops->assemble(K,cr,r,cc,c,v);
    }
    void AddToVector(int,double) {}
    void AddToVector(int cr,int * r,double * v)
    {
      ops->assemble(f_i,cr,r,v);
    }
    void solve()
    {
      slv->solve(K,u_i,f_i);
      ops->axpy(1.0,u_i,u);
      ops->axpy(1.0,f_i,f);
    }
    bool Zero()
    {
      ZeroMatrix();
      ZeroVector();
      return true;
    }
    bool ZeroMatrix()
    {
      ops->zero(K);
      return true;
    }
    bool ZeroVector()
    {
      ops->zero(f_i);
      return true;
    }
    void GetVector(double *& f)
    {
      ops->get(f_i,f);
    }
    void SetVector(const double * f)
    {
      ops->restore(f_i,(double*&)f);
    }
    void GetVectorNorm(double & n)
    {
      n = ops->norm(f_i);
    }
    void GetAccumVector(double *& fa)
    {
      ops->get(f,fa);
    }
    void GetAccumVectorNorm(double & n)
    {
      n = ops->norm(f);
    }
    void GetPrevVector(double *& f)
    {
      ops->get(f_i1,f);
    }
    void GetPrevVectorNorm(double & n)
    {
      n = ops->norm(f_i1);
    }
    void GetDotNorm(double & n)
    {
      n = std::fabs(ops->dot(f_i,u_i));
    }
    void GetPrevDotNorm(double & n)
    {
      n = std::fabs(ops->dot(f_i1,u_i1));
    }
    void GetAccumDotNorm(double & n)
    {
      n = std::fabs(ops->dot(f,u));
    }
    void GetSolution(double *& u)
    {
      ops->get(u_i,u);
    }
    void GetSolutionNorm(double & n)
    {
      n = ops->norm(u_i);
    }
    void GetAccumSolution(double *& ua)
    {
      ops->get(u,ua);
    }
    void GetAccumSolutionNorm(double & n)
    {
      n = ops->norm(u);
    }
    void GetPrevSolution(double *& u)
    {
      ops->get(u_i1,u);
    }
    void GetPrevSolutionNorm(double & n)
    {
      n = ops->norm(u_i1);
    }
    void PrintMatrix(std::ostream &) {}
    void PrintVector(std::ostream &) {}
    void PrintSolution(std::ostream &) {}
    double MatrixMax() { return 0.0; }
    int GlobalDOFs() { return glb_dofs; }
    int LocalDOFs() { return lcl_dofs; }
    int LocalOffset() { return vec_low; }
  };
}
#endif
