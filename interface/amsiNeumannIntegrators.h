#ifndef AMSI_NEUMANN_INTEGRATORS_H_
#define AMSI_NEUMANN_INTEGRATORS_H_
#include "amsiBoundaryConditions.h"
#include "apfFunctions.h"
#include <apf.h>
#include <apfDynamicVector.h>
namespace amsi
{
  class NeummanIntegrator : public apf::Integrator
  {
  protected:
    apf::DynamicVector fe;
    apf::Field * fld;
    apf::MeshElement * me;
    apf::Element * e;
    int nedofs;
    int nenodes;
    int nfcmps;
    double tm;
    std::vector<double> vls;
    BCQuery * qry;
  public:
    NeumannIntegrator(apf::Field * f, int o, BCQuery * q)
      : apf::Integrator(o)
      , fe()
      , fld(f)
      , me()
      , e()
      , nedofs()
      , nenodes()
      , nfcmps(apf::countComponents(fld))
      , t()
      , vls(nfcmps)
      , qry(q)
    { }
    void setTime(double t)
    {
      tm = t;
    }
    virtual void inElement(apf::MeshElement * m)
    {
      me = m;
      e = apf::createElement(fld,me);
      nenodes = apf::countNodes(e);
      assert(nfcmp == qry->numComps());
      int nnedofs = nenodes * nfcmp;
      if(nnedofs != nedofs)
        fe.setSize(nnedofs);
      nedofs = nnedofs;
      fe.zero();
    }
    void integrate(double (*intop)(int,int))
    {
      for(int ii = 0; ii < nenodes; ii++)
        for(int jj = 0; jj < nfcmps; jj++)
          fe(ii*nfcmps + jj) = (*intop)(ii,jj);
    }
    virtual void outElement()
    {
      // maybe just assemble here...
    }
  protected:
    void updateBCQueryValues(apf::Vector3 const & p)
    {
      if(qry->isConst())
        for(int ii = 0; ii < nfcmps; ii++)
          vls[ii] = qry->getValue(ii);
      else if(qry->isSpaceExpr())
      {
        apf::Vector3 xyz;
        apf::mapLocalToGlobal(me,p,xyz);
        if(qry->isTimeExpr())
          for(int ii = 0; ii < nfcmps; ii++)
            vls[ii] = qry->getValue(ii,tm,x,y,z);
        else
          for(int ii = 0; ii < nfcmps; ii++)
            vls[ii] = qry->getValue(ii,x,y,z);
      }
      else if(qry->isTimeExpr())
        for(int ii = 0; ii < nfcmps; ii++)
          vls[ii] = qry->getValue(ii,tm);
    }
  };
  class SurfaceTraction : public NeumannIntegrator
  {
  public:
    SurfaceTraction(apf::Field * f, int o, BCQuery * q)
      : NeumannIntegrator(f,o,q)
    { }
    void atPoint(apf::Vector3 const & p, double w, double dV)
    {
      updateBCQueryValues(p);
      apf::NewArray<double> N;
      apf::getShapeValues(e,p,N);
      double wxdV = w * dV;
      auto intop = [&](int ii, int jj) -> double {return N[ii] * F[jj] * wxdV};
      integrate(intop);
    }
  };
  class Pressure : public NeumannIntegrator
  {
  private:
    apf::Mesh * msh;
    apf::MeshEntity * ent;
  public:
    Pressure(apf::Field * f, int o, BCQuery * q)
      : NeumannIntegrator(f,o,q)
      , msh(apf::getMesh(f))
    { }
    void inElement(apf::MeshElement * m)
    {
      NeumannIntegrator::inElement(m);
      ent = apf::getMeshEntity();
    }
    void atPoint(apf::Vector3 const & p, double w, double dV)
    {
      updateBCQueryValues(p);
      apf::NewArray<double> N;
      apf::Vector3 nrml;
      Analysis::faceNormal(msh,ent,nrml);
      vls[0] *= nrml.x();
      vls[1] *= nrml.y();
      vls[2] *= nrml.z();
      double wxdV = w * dV;
      auto intop = [&](int ii, int jj) -> double { return N[ii] * vls[jj] * wxdV; }
      integrate(intop);
    }
  };
}
#endif