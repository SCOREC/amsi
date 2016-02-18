#ifndef CONVENIENCEFUNCTIONS_H_
#define CONVENIENCEFUNCTIONS_H_
#include <apf.h>
#include <apfDynamicMatrix.h>
#include <apfDynamicVector.h>
namespace amsi
{
  extern const apf::Matrix3x3 Identity3x3;
  extern const int Voigt3x3[3][3];
  void FiberAxialForce(double A, double B,
                       double stretch_ratio,
                       double init_length,
                       double & dforce_dlength,
                       double & fiber_force);
  void DeformationGradient(apf::Element * e,
                           const apf::Vector3 & p,
                           apf::Matrix3x3 & grad,
                           double & grad_det);
  void LeftCauchy(const apf::Matrix3x3 & F,
                  apf::Matrix3x3 & B);
  void RightCauchy(const apf::Matrix3x3 & F,
                   apf::Matrix3x3 & C);
  void LinearizedConstitutive(const apf::Matrix3x3 & C,
                              double J,
                              double poisson_ratio,
                              double shear_modulus,
                              apf::Matrix<6,6> & strain);
  void PK2Stress(const apf::Matrix3x3 & C,
                 double J,
                 double poisson_ratio,
                 double shear_modulus,
                 apf::Matrix3x3 & pk2);
  void PrintElementalSystem(std::ostream & stream,
                            int num_elemental_dofs,
                            const int * dof_numbers,
                            const double * Ke,
                            const double * fe);
  double* DynamicMatrix2Double(apf::DynamicMatrix&);
  double* DynamicVector2Double(apf::DynamicVector&);
}
#endif
