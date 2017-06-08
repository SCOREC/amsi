#ifndef NONLINELASTICINTEGRATOR_H_
#define NONLINELASTICINTEGRATOR_H_
#include "ElementalSystem.h"
namespace amsi
{
  class NonLinElasticIntegrator : public ElementalSystem
  {
  public:
    NonLinElasticIntegrator(apf::Field * f, int o, double E, double v);
    void atPoint(apf::Vector3 const &p, double w, double dV);
    void setStressField(apf::Field* f) {stress_ip_field = f;}
    void setStrainField(apf::Field* f) {strain_ip_field = f;}
  private:
    void getDisplacements(apf::DynamicVector&);
    apf::Field * stress_ip_field;
    apf::Field * strain_ip_field;
    apf::DynamicMatrix C;
    double v;
    double E;
  };
}
#endif
