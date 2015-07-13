#include "NonLinElasticIntegrator.h"
#include "LinearElasticIntegrator.h"
#include "ConvenienceFunctions.h"

namespace amsi
{
  namespace Analysis
  {

    NonLinElasticIntegrator::NonLinElasticIntegrator(apf::Field * field,
						     int o,
						     double youngs_modulus,
						     double poisson_ratio) :
      ElementalSystem(field,o),
      D(GetIsotropicStressStrainTensor(youngs_modulus,poisson_ratio))
    {   }

    double delta(int i, int j)
    {
      return i == j ? 1.0 : 0.0;
    }

    double NonLinElasticIntegrator::C(int i, int j, int k, int l)
    {
      double lambda = (E*v)/((1+v)*(1-2*v));
      double mu = E / (2 * (1 + v));
      return lambda * delta(i,j) * delta(k,l) + mu * (delta(i,k)*delta(j,l) + delta(i,l)*delta(j,k));
    }

    void NonLinElasticIntegrator::getDisplacements(apf::DynamicVector & u)
    {
      apf::Element * e_disp = apf::createElement(f,me);
      apf::NewArray<apf::Vector3> disp;
      apf::getVectorNodes(e_disp,disp);
      for(int ii=0;ii<nedofs;ii++)
	u(ii) = disp[ii/3][ii%3];
    }

    void NonLinElasticIntegrator::atPoint(apf::Vector3 const & p, double w, double dV)
    {
      apf::NewArray<apf::Vector3> grads;
      apf::getShapeGrads(e,p,grads);

      // Linear strain-displacement matrix see Bathe pgs 555-556
      apf::DynamicMatrix BL(6,nedofs);
      for(int ii = 0; ii < nenodes; ii++)
      {
	BL(0,3*ii)   = grads[ii][0]; // N_(ii,1)
	BL(0,3*ii+1) = BL(0,3*ii+2) = 0.0;
	
	BL(1,3*ii+1) = grads[ii][1]; // N_(ii,2)
	BL(1,3*ii)   = BL(1,3*ii+2) = 0.0;
	
	BL(2,3*ii+2) = grads[ii][2]; // N_(ii,3)
	BL(2,3*ii)   = BL(2,3*ii+1) = 0.0;
	
	BL(3,3*ii)   = grads[ii][1]; // N_(ii,2)
	BL(3,3*ii+1) = grads[ii][0]; // N_(ii,1)
	BL(3,3*ii+2) = 0.0;
	
	BL(4,3*ii)   = 0.0;
	BL(4,3*ii+1) = grads[ii][2]; // N_(ii,3)
	BL(4,3*ii+2) = grads[ii][1]; // N_(ii,2)
	
	BL(5,3*ii)   = grads[ii][2]; // N_(ii,3)
	BL(5,3*ii+1) = 0.0;
	BL(5,3*ii+2) = grads[ii][0]; // N_(ii,1)
      }

      apf::DynamicMatrix BNL(9,nedofs); //nonlinear strain disp
      BNL *= 0.0;
      double Bp[3][10] = {}; // hard-coded, need to fix this
      
      for(int ii = 0; ii < 3; ii++)
	for(int jj = 0; jj < 4; jj++)
	  Bp[ii][jj*3] = grads[jj][ii];
      
      for(int ii = 0; ii < 3; ii++)
	for(int jj = 0; jj < 10; jj++)
	{
	  BNL(ii,jj) = Bp[ii][jj];
	  BNL(3+ii,jj+1) = Bp[ii][jj];
	  BNL(6+ii,jj+2) = Bp[ii][jj];
	}
      
      apf::DynamicMatrix K0(nedofs,nedofs);
      apf::DynamicMatrix DB(6,nedofs);
      apf::multiply(D,BL,DB);
      apf::DynamicMatrix BT(nedofs,6);
      apf::transpose(BL,BT);
      apf::multiply(BT,DB,K0);

      apf::DynamicVector t_u(nedofs);
      getDisplacements(t_u);
      
      apf::DynamicVector strain(6);
      apf::multiply(BL,t_u,strain);
      apf::DynamicVector cauchy_stressv(6);
      apf::multiply(D,strain,cauchy_stressv);

      apf::DynamicMatrix K1(nedofs,nedofs);
      apf::DynamicMatrix tau(9,9); // cauchy stresses in special matrix
      tau.zero();
      
      for(int ii = 0; ii < 3; ii++)
	for(int jj = 0; jj < 3; jj++)
	  tau(ii,jj) = tau(ii+3,jj+3) = tau(ii+6,jj+6) = cauchy_stressv(Voigt3x3[ii][jj]);
      
      apf::DynamicMatrix TAUxBNL;
      apf::multiply(tau,BNL,TAUxBNL);
      apf::DynamicMatrix BNLT;
      apf::transpose(BNL,BNLT);
      apf::multiply(BNLT,TAUxBNL,K1);
      
      K0 += K1;
      K0 *= w*dV;
      Ke += K0;
    }
  }
}
