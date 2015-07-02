#ifndef H_NONLINFINITEELEMENT
#define H_NONLINFINITEELEMENT

#include "apf.h"
#include "apfDynamicMatrix.h"

namespace amsi {
  namespace Analysis {

  void LinearStiffnessK0(int num_nodes,
			 apf::DynamicMatrix & B0, 
			 apf::Matrix<6,6> & hookes, 
			 apf::DynamicMatrix & K0);

  void NonLinearStiffness(int num_nodes,
			  apf::DynamicMatrix & BNL, 
			  apf::Matrix3x3 & pk2, 
			  apf::DynamicMatrix & KNL);

  void LinearStrainDisp(int num_nodes,
			apf::NewArray<apf::Vector3> & shape_deriv, 
			apf::NewArray<apf::Vector3> & node_disp, 
			apf::DynamicMatrix & BL);

  void LinearStrainDispB0(int num_nodes,
			  apf::NewArray<apf::Vector3> & shape_deriv, 
			  apf::DynamicMatrix & B);

  void LinearStrainDispB1(int num_nodes, 
			  apf::NewArray<apf::Vector3> & shape_deriv, 
			  apf::NewArray<apf::Vector3> & node_disp,
			  apf::DynamicMatrix & B); 

  void ComputeGreenStrain(apf::DynamicMatrix & H, 
			  apf::DynamicVector & y, 
			  apf::Matrix3x3 E);

  void ComputePK2Stress(apf::DynamicMatrix & H, 
			apf::DynamicVector & S, 
			apf::DynamicMatrix & C);

  void NonLinearStrainDisp(int num_nodes,
			   apf::NewArray<apf::Vector3> & shape_deriv, 
			   apf::DynamicMatrix & B);

  }
}
#endif






















































