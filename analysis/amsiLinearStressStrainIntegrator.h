#ifndef AMSI_LINEAR_STRESS_STRAIN_INTEGRATOR_H_
#define AMSI_LINEAR_STRESS_STRAIN_INTEGRATOR_H_
#include "ElementalSystem.h"
namespace amsi
{
  class LinearStressStrainIntegrator : public ElementalSystem
  {
  public:
    LinearStressStrainIntegrator(apf::Field * u, apf::Field * stn, apf::Field * sts, double E, double v);
    void inElement(apf::MeshElement * melm);
    void outElement();
    void atPoint(apf::Vector3 const & p, double w, double dV);
  private:
    apf::DynamicMatrix C;
    apf::NewArray<apf::Vector3> u;
    apf::DynamicVector * U;
    apf::Field * strs;
    apf::Field * strn;
  };
}
#endif
