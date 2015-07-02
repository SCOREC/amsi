#ifndef STRESSSTRAININTEGRATOR_H_
#define STRESSSTRAININTEGRATOR_H_

#include "ElementalSystem.h"

namespace amsi {
  namespace Analysis {

    class LinearStressStrainIntegrator : public ElementalSystem
    {
    public:
      LinearStressStrainIntegrator(apf::Field * f,
				   apf::Field * strain_ip_field,
				   apf::Field * stress_ip_field,
				   double E, double v);

      void inElement(apf::MeshElement * melm);
      void outElement();
      
      void atPoint(apf::Vector3 const & p,
		   double w,
		   double dV);
    private:
      apf::DynamicMatrix & D;
      apf::NewArray<apf::Vector3> u;
      apf::DynamicVector * U;

      apf::Field * stress_field;
      apf::Field * strain_field;
    };
    
  }
}

#endif
