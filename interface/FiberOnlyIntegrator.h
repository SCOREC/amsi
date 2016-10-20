#ifndef BIO_FO_INTEGRATOR_H_
#define BIO_FO_INTEGRATOR_H_
#include <apf.h>
#include <amsiFEA.h>
namespace bio
{
  void makeS(apf::DynamicVector & s,
             apf::DynamicMatrix & S)
  {
    S.resize(9,9);
    S.zero();
    S(0,0) = S(3,3) = S(6,6) = s(0);
    S(0,1) = S(3,4) = S(6,7) = s(1);
    S(0,2) = S(3,5) = S(6,8) = s(2);
    S(1,0) = S(4,3) = S(7,6) = s(1);
    S(1,1) = S(4,4) = S(7,7) = s(3);
    S(1,2) = S(4,5) = S(7,8) = s(4);
    S(2,0) = S(5,3) = S(8,6) = s(2);
    S(2,1) = S(5,4) = S(8,7) = s(4);
    S(2,2) = S(5,5) = S(8,8) = s(5);
  }
  class FiberOnlyIntegrator : public apf::Integrator
  {
  protected:
    amsi::ElementalSystem es;
    apf::Field * du;
    apf::Field * rve;
    apf::Element * e;
    int dim;
    int nedofs;
    int nenodes;
    int gid;
  public:
    FiberOnlyIntegrator(int o,
                        apf::Field * u,
                        apf::Field * r)
      : apf::Integrator(o)
      , du(u)
      , rve(r)
      , es()
      , e(NULL)
      , dim(0)
      , nedof(0)
      , nenode(0)
      , gid(0)
    {
      assert(f);
    }
    void inElement(apf::MeshElement * melmnt)
    {
      e = apf::createElement(fld,melmnt);
      nenode = apf::countNodes(e);
      dim = apf::getDimension(melmnt);
      nedof = nenode * dim; //eh....
      es.resize(nedof);
      gid = 0;
    }
    void outElement();
    void atPoint(apf::Vector3 const & p, double w, double dV)
    {
      //rveinfo * rve_info = ;
      apf::NewArray<apf::Vector3> dN_dxi;
      apf::getShapeGrads(e,p,dN_dxi);
      apf::DynamicMatrix BL(6,nenode);
      amsi::linearStrainDisp(dN_dxi,nenode,BL);
      apf::DynamicMatrix BNL(9,nedof);
      amsi::nonlinearStrainDisp(dN_dxi,nenode,BNL);
      apf::DynamicVector SV;
      fromArray(SV,&rve_info.s[0]);
      apf::DynamicVector Q;
      fromArray(Q,&rve_info.q[0]);
      apf::DynamicMatrix S;
      makeS(SV,S);
      apf::DynamicMatrix BLT;
      apf::transpose(BL,BLT);
      apf::DynamicMatrix BNLT;
      apf::transpose(BNL,BNLT);
      apf::DynamicMatrix BNLTxS;
      BNLTxS = BNLT * S;
      apf::DynamicVector BLTxSV;
      BLTxSV = BLT * SV;
      apf::DynamicMatrix K1(nedof,dedof);
      apf::multiply(BNLTxS,BNL,K1);
      apf::DynamicMatrix K0(nedof,nedof);
      apf::multiply(BLT,dS_dfe,K0);
      es.getKe() += w*dV * (K0 + K1);
      gid++;
    }
  };
}
#endif
