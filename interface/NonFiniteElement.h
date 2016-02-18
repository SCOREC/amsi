#ifndef H_NONLINFINITEELEMENT
#define H_NONLINFINITEELEMENT
#include "apf.h"
#include "apfDynamicMatrix.h"
namespace amsi
{
  void linearStiffnessK0(int num_nodes,
                         apf::DynamicMatrix & B0,
                         apf::Matrix<6,6> & hookes,
                         apf::DynamicMatrix & K0);
  void nonLinearStiffness(int num_nodes,
                          apf::DynamicMatrix & BNL,
                          apf::Matrix3x3 & pk2,
                          apf::DynamicMatrix & KNL);
  void linearStrainDisp(int num_nodes,
                        apf::NewArray<apf::Vector3> & shape_deriv,
                        apf::NewArray<apf::Vector3> & node_disp,
                        apf::DynamicMatrix & BL);
  void linearStrainDispB0(int num_nodes,
                          apf::NewArray<apf::Vector3> & shape_deriv,
                          apf::DynamicMatrix & B);
  void linearStrainDispB1(int num_nodes,
                          apf::NewArray<apf::Vector3> & shape_deriv,
                          apf::NewArray<apf::Vector3> & node_disp,
                          apf::DynamicMatrix & B);
  void computeGreenStrain(apf::DynamicMatrix & H,
                          apf::DynamicVector & y,
                          apf::Matrix3x3 E);
  void computePK2Stress(apf::DynamicMatrix & H,
                        apf::DynamicVector & S,
                        apf::DynamicMatrix & C);
  void nonLinearStrainDisp(int num_nodes,
                           apf::NewArray<apf::Vector3> & shape_deriv,
                           apf::DynamicMatrix & B);
}
#endif






















































