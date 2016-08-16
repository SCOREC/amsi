#ifndef APFFUNCTIONS_H_
#define APFFUNCTIONS_H_
#include <apf.h>
#include <apfDynamicVector.h>
#include <apfField.h>
#include <apfNumbering.h>
#include <cstring>
#include <cmath>
#include <ostream>
namespace amsi
{
  template <typename I>
    void setEntitiesNode(apf::Field * fld, double vl, I bgn, I nd);
  // move below here into a fieldops-specific header or something
  /**
   * The way this is currently structured sucks as the component loop is in the applier classes, so the
   *  apply op, which involves at least one virtual function lookup, is called multiple times per node,
   *  it was already bad enough that it was called for each node in the mesh, but now it is also called
   *  (again at least once, sometimes twice for FreeApplyOp wrapped ops) for every component of every
   *  node in the mesh...
   */
  class ApplyOp
  {
  protected:
    apf::FieldOp * aplr;
  public:
    ApplyOp()
      : aplr(NULL)
    { }
    void setApplier(apf::FieldOp * a) {aplr = a;}
    virtual void apply(apf::MeshEntity * me, int nde, int cmp, int cmps, double & to, const double & frm) = 0;
  };
  /**
   * A wrapper class that takes another apply operation and only applies it if the dof being written to
   *  is a free dof.
   */
  class FreeApplyOp : public ApplyOp
  {
  protected:
    apf::Numbering * nm;
    ApplyOp * op;
  public:
    FreeApplyOp(apf::Numbering * n, ApplyOp * p)
      : nm(n)
      , op(p)
    { }
    virtual void apply(apf::MeshEntity * me, int nde, int cmp, int cmps, double & to, const double & frm)
    {
      if(!apf::isFixed(nm,me,nde,cmp))
        op->apply(me,nde,cmp,cmps,to,frm);
    }
  };
  class WriteOp : public ApplyOp
  {
  public:
    virtual void apply(apf::MeshEntity*, int, int, int, double & to, const double & frm)
    {
      to = frm;
    }
  };
  class SquareApplyOp : public ApplyOp
  {
  protected:
    ApplyOp * op;
  public:
    SquareApplyOp(ApplyOp * o)
      : op(o)
    {}
    virtual void apply(apf::MeshEntity * me, int nde, int cmp, int cmps, double & to, const double & frm)
    {
      double sqrd = frm * frm;
      op->apply(me,nde,cmp,cmps,to,sqrd);
    }
  };
  class AccumOp : public ApplyOp
  {
  public:
    virtual void apply(apf::MeshEntity*, int, int, int, double & to, const double & frm)
    {
      to += frm;
    }
  };
  class WriteNZOp : public ApplyOp
  {
  private:
    double eps;
  public:
    WriteNZOp(double e)
      : eps(e)
    { }
    virtual void apply(apf::MeshEntity*, int, int, int, double & to, const double & frm)
    {
      to = std::abs(frm) < eps ? to : frm;
    }
  };
  class ExtractOp : public apf::FieldOp
  {
  protected:
    apf::Field * frm;
    int fldcmp;
    double to;
    double * frmdofs;
    ApplyOp * op;
    apf::MeshEntity * me;
  public:
    ExtractOp(apf::Field * f, ApplyOp * p)
      : frm(f)
      , fldcmp(apf::countComponents(frm))
      , to()
      , frmdofs(new double[fldcmp])
      , op(p)
      , me()
    {
      op->setApplier(this);
    }
    ~ExtractOp()
    {
      delete [] frmdofs;
    }
    bool inEntity(apf::MeshEntity * e)
    {
      me = e;
      return true;
    }
    void atNode(int nde)
    {
      memset(&frmdofs[0],0.0,sizeof(double)*fldcmp);
      apf::getComponents(frm,me,nde,&frmdofs[0]);
      for(int ii = 0; ii < fldcmp; ii++)
        op->apply(me,nde,ii,fldcmp,to,frmdofs[ii]);
    }
    void outEntity()
    {
      me = NULL;
    }
    void run() { apply(frm); }
    double getExtractedValue() { return to; }
  };
  /**
   * Apply an ApplyOp to a source and destination field.
   *  Can be used for copying, accumulating, etc.
   */
  class MergeFields : public apf::FieldOp
  {
  protected:
    apf::Field * to;
    apf::Field * frm;
    int fldcmp;
    double * frmdofs;
    double * todofs;
    apf::MeshEntity * me;
    ApplyOp * op;
  public:
    MergeFields(apf::Field * t, apf::Field * f, ApplyOp * p)
      : to(t)
      , frm(f)
      , fldcmp(apf::countComponents(f))
      , frmdofs(new double[fldcmp])
      , todofs(new double[fldcmp])
      , me(NULL)
      , op(p)
    {
      op->setApplier(this);
    }
    ~MergeFields()
    {
      delete [] frmdofs;
      delete [] todofs;
    }
    bool inEntity(apf::MeshEntity * e)
    {
      me = e;
      return true;
    }
    void atNode(int nde)
    {
      memset(&frmdofs[0],0.0,sizeof(double)*fldcmp);
      memset(&todofs[0],0.0,sizeof(double)*fldcmp);
      apf::getComponents(frm,me,nde,&frmdofs[0]);
      apf::getComponents(to,me,nde,&todofs[0]);
      for(int ii = 0; ii < fldcmp; ii++)
        op->apply(me,nde,ii,fldcmp,todofs[ii],frmdofs[ii]);
      apf::setComponents(to,me,nde,&todofs[0]);
    }
    void outEntity()
    {
      me = NULL;
    }
    void run() { apply(to); }
  };
  /**
   * Takes a numbering and array of doubles with an initial dof offset and either
   *  sets or accumulates the array values at the index corresponding to the
   *  numbering minus the initial dof offset.
   */
  class ApplyVector : public apf::FieldOp
  {
  protected:
    apf::Numbering * nm;
    apf::Field * fld;
    apf::MeshEntity * me;
    const double * dofs;
    int fldcmp;
    int dof0;
    double * cmps;
    ApplyOp * op;
  public:
    ApplyVector(apf::Numbering * n, apf::Field * f, const double * s, int d, ApplyOp * o)
      : apf::FieldOp()
      , nm(n)
      , fld(f)
      , me(NULL)
      , dofs(s)
      , fldcmp(apf::countComponents(fld))
      , dof0(d)
      , cmps(new double[fldcmp])
      , op(o)
    { }
    ~ApplyVector()
    {
      delete [] cmps;
    }
    bool inEntity(apf::MeshEntity * m) { me = m; return apf::getMesh(fld)->isOwned(me);}
    void outEntity() {}
    void atNode(int nde)
    {
      memset(&cmps[0],0.0,sizeof(double)*fldcmp);
      apf::getComponents(fld,me,nde,&cmps[0]);
      for(int ii = 0; ii < fldcmp; ii++)
        if(apf::isNumbered(nm,me,nde,ii))
        {
          int dof = apf::getNumber(nm,me,nde,ii);
          op->apply(me,nde,ii,fldcmp,cmps[ii],dofs[dof - dof0]);
        }
      apf::setComponents(fld,me,nde,&cmps[0]);
    }
    void run() { apply(fld); }
  };
  /*
  class RecoverVector : public apf::FieldOp
  {
  protected:
    apf::MeshEntity * me;
    apf::Numbering * nm;
    apf::Field * fld;
    std::vector<double> vec;
    int fld_cmps;
    double * cmps;
  public:
  ConstructVector(apf::Numbering * n, int sz)
      : me(NULL)
      , nm(n)
      , vec(sz)
      , fld_cmps(apf::countComponents(apf::getField(n)))
      , cmps(new double [fld_cmps])
    { }
    ~ConstructVector()
    {
      delete [] cmps;
    }
    bool inEntity(apf::MeshEntity * m) { me = m; return true; }
    void outEntity() {}
    void atNode(int nde)
    {
      memset(&cmps,0.0,sizeof(double)*fld_cmps);
      apf::getComponents(fld,me,nde,&cmps[0]);
      if(apf::isNumbered())
    }
    double * getVector()
    {
      return &vec[0];
    }
      vod
  };
  */
  void faceNormal(apf::Mesh *,
                  apf::MeshEntity *,
                  apf::Vector3 & n);
  void faceNormal(const apf::Vector3 & pt_a,
		  const apf::Vector3 & pt_b,
		  const apf::Vector3 & pt_c,
		  apf::Vector3 & normal);
  void vertexNormal(apf::Mesh *,
                    apf::MeshEntity *,
                    apf::Vector3 & n);
  double edgeLength(const apf::Vector3 & pt_a, const apf::Vector3 & pt_b);
  double triangleArea(const apf::Vector3 & pt_a,
		      const apf::Vector3 & pt_b,
		      const apf::Vector3 & pt_c);
  void displaceMesh(apf::Field * displacement_field);
  void printNumbering(std::ostream & out, apf::Numbering * numbering);
  void SymmMatrixToVoigtVector(const apf::Matrix3x3 & mat, apf::Vector<6> & vec);
  void VoigtVectorToSymmMatrix(const apf::Vector<6> & vec, apf::Matrix3x3 & mat);
  void VoigtVectorToSymmMatrix(const apf::DynamicVector & vec, apf::Matrix3x3 & mat);
  double measureDisplaced(apf::MeshEntity * ment, apf::Field * u);
  double measureDisplacedFromSurf(apf::MeshEntity * ment, apf::Field * u, int norm_dir);
}
#include "apfFunctions_impl.h"
#endif
