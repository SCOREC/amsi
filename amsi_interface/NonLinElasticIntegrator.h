#ifndef NONLINELASTICINTEGRATOR_H_
#define NONLINELASTICINTEGRATOR_H_


#include "ElementalSystem.h"

namespace amsi
{
  namespace Analysis
  {
    class NonLinElasticIntegrator : public ElementalSystem
    {
    public:
      NonLinElasticIntegrator(apf::Field * field,
			      int o,
			      double youngs_modulus,
			      double poisson_ratio);
      
      void atPoint(apf::Vector3 const &p, double w, double dV);
    private:
      void getDisplacements(apf::DynamicVector&);
      double C(int,int,int,int);
      apf::DynamicMatrix& D;
      double v;
      double E;
    };
  }
}

#endif
