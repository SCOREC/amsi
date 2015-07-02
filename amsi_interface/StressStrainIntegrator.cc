#include "StressStrainIntegrator.h"

#include "LinearElasticIntegrator.h"
#include "apfFunctions.h"

namespace amsi {
  namespace Analysis {

    LinearStressStrainIntegrator::LinearStressStrainIntegrator(apf::Field * f,
							       apf::Field * strain_ip_field,
							       apf::Field * stress_ip_field,
							       double E, double v) :
      ElementalSystem(f,0),
      D(GetIsotropicStressStrainTensor(E,v)),
      u(),U(),
      stress_field(stress_ip_field),
      strain_field(strain_ip_field)
    {}

    void LinearStressStrainIntegrator::inElement(apf::MeshElement * melm)
    {
      ElementalSystem::inElement(melm);
      U = new apf::DynamicVector(nenodes*num_field_components);
      apf::getVectorNodes(e,u);
      for(int ii = 0; ii < nenodes; ii++)
	for(int jj = 0; jj < num_field_components; jj++)
	  (*U)[ii*num_field_components + jj] = u[ii][jj];
    }
    
    void LinearStressStrainIntegrator::atPoint(apf::Vector3 const & p,
					       double w, double dV)
    {
      apf::NewArray<apf::Vector3> grads;
      apf::getShapeGrads(e,p,grads);
      
      // Linear strain-displacement matrix see Bathe pgs 555-556
      apf::DynamicMatrix B(6,nedofs);
      for(int ii = 0; ii < nenodes; ii++)
      {
	B(0,3*ii)   = grads[ii][0]; // N_(ii,1)
	B(0,3*ii+1) = B(0,3*ii+2) = 0.0;
	
	B(1,3*ii+1) = grads[ii][1]; // N_(ii,2)
	B(1,3*ii)   = B(1,3*ii+2) = 0.0;
	
	B(2,3*ii+2) = grads[ii][2]; // N_(ii,3)
	B(2,3*ii)   = B(2,3*ii+1) = 0.0;
	
	B(3,3*ii)   = grads[ii][1]; // N_(ii,2)
	B(3,3*ii+1) = grads[ii][0]; // N_(ii,1)
	B(3,3*ii+2) = 0.0;
	
	B(4,3*ii)   = 0.0;
	B(4,3*ii+1) = grads[ii][2]; // N_(ii,3)
	B(4,3*ii+2) = grads[ii][1]; // N_(ii,2)
	
	B(5,3*ii)   = grads[ii][2]; // N_(ii,3)
	B(5,3*ii+1) = 0.0;
	B(5,3*ii+2) = grads[ii][0]; // N_(ii,1)
      }

      apf::DynamicVector strain(6);
      apf::DynamicVector stress(6);

      apf::multiply(B,(*U),strain);
      apf::multiply(D,strain,stress);

      apf::Matrix3x3 epsilon;
      apf::Matrix3x3 sigma;

      VoigtVectorToSymmMatrix(strain,epsilon);
      VoigtVectorToSymmMatrix(stress,sigma);

      apf::setMatrix(strain_field,apf::getMeshEntity(me),0,epsilon);
      apf::setMatrix(stress_field,apf::getMeshEntity(me),0,sigma);
    }

    void LinearStressStrainIntegrator::outElement()
    {
      delete U;
    }
    
  }
}
