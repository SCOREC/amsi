#ifndef NONLINELASTICINTEGRATOR_H_
#define NONLINELASTICINTEGRATOR_H_
#include "ElementalSystem.h"
namespace amsi
{
  class NonLinElasticIntegrator : public ElementalSystem
  {
  public:
    NonLinElasticIntegrator(apf::Field * field,
                            int o,
                            double youngs_modulus,
                            double poisson_ratio);
    void atPoint(apf::Vector3 const &p, double w, double dV);
    void setStressField(apf::Field* f) {stress_ip_field = f;}
    void setStrainField(apf::Field* f) {strain_ip_field = f;}
  private:
    void getDisplacements(apf::DynamicVector&);
    double C(int,int,int,int);
    apf::Field * stress_ip_field;
    apf::Field * strain_ip_field;
    apf::DynamicMatrix& D;
    double v;
    double E;
  };
}
#endif
