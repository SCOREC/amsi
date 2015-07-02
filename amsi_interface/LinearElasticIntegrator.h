#ifndef LINEARELASTICINTEGRATOR_H_
#define LINEARELASTICINTEGRATOR_H_

#include "ElementalSystem.h"

namespace amsi {
  namespace Analysis {

    apf::DynamicMatrix & GetIsotropicStressStrainTensor(double E, double v);
    
    class LinearElasticIntegrator : public ElementalSystem
    {
    public: 
      LinearElasticIntegrator(apf::Field * field,
			      int o,
			      double E,
			      double v);
      void atPoint(apf::Vector3 const &p,
		   double w,
		   double dV);
    private:
      apf::FieldShape * fs;
      apf::DynamicMatrix & D;
    };
    
  }
}

#endif
