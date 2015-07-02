#include "ConvenienceFunctions.h"

namespace amsi {
  namespace Analysis {

    const apf::Matrix3x3 Identity3x3(1.0,0.0,0.0,
				     0.0,1.0,0.0,
				     0.0,0.0,1.0);
    
    const int Voigt3x3[3][3] = {{0,5,4},
				{5,1,3},
				{4,3,2}};
    
    void FiberAxialForce(double A, double B,
			 double stretch_ratio, 
			 double init_length, 
			 double & dforce_dlength, 
			 double & fiber_force)
    {
      double ratio_limit = 1.4;
      double ratio = stretch_ratio > ratio_limit ? ratio_limit : stretch_ratio;
      
      double exp_term = exp(B * (ratio * ratio - 1));
      double ratio_sqrd = ratio * ratio;
      
      // = A^2 s^2 exp(B (s^2 - 1))
      fiber_force = (A * ratio_sqrd) * exp_term;
      
      // = 2 A s/l ( exp(B (s^2 - 1)) - 1 + B s^2 exp(B (s^2 - 1)) )
      dforce_dlength = 2 * A * (ratio / init_length) * 
	( exp_term - 1 + B * ratio_sqrd * exp_term );
    }
    
    void DeformationGradient(apf::Element * e,
			     const apf::Vector3 & p,
			     apf::Matrix3x3 & grad,
			     double & grad_det)
    {
      apf::NewArray<apf::Vector3> u;
      apf::getVectorNodes(e,u);

      apf::NewArray<apf::Vector3> shape_derivs;
      apf::getShapeGrads(e,p,shape_derivs);

      grad = Identity3x3;

      int num_nodes = apf::countNodes(e);
      int field_components = 3; // assumption
      for(int ii = 0; ii < field_components; ii++)
	for(int jj = 0; jj < field_components; jj++)
	  for(int kk = 0; kk < num_nodes; kk++)
	    grad[ii][jj] += u[kk][ii] * shape_derivs[kk][jj];
      
      grad_det = apf::getDeterminant(grad);
    }

    void LeftCauchy(const apf::Matrix3x3 & F, 
		    apf::Matrix3x3 & B) 
    {
      for(int ii = 0; ii < 3; ii++)
	for(int jj = 0; jj < 3; jj++)
	{
	  B[ii][jj] = 0.0;
	  for(int kk = 0; kk < 3; kk++)
	    B[ii][jj] += F[ii][kk] * F[jj][kk]; 
	}
    }      

    void RightCauchy(const apf::Matrix3x3 & F,
		     apf::Matrix3x3 & C)
    {
      for(int ii = 0; ii < 3; ii++)
	for(int jj = 0; jj < 3; jj++)
	{
	  C[ii][jj] = 0.0;
	  for(int kk = 0; kk < 3; kk++)
	    C[ii][jj] += F[kk][ii] * F[kk][jj];
	}
    }

    void LinearizedConstitutive(const apf::Matrix3x3 & C,
				double J,
				double poisson_ratio,
				double shear_modulus,
				apf::Matrix<6,6> & strain)
    {
      // shear modulus = mu (lame constant)
      
      // use shear modulus and poisson's ratio as material constant
      double beta = poisson_ratio / ( 1 - 2 * poisson_ratio);
      double c1 = shear_modulus * 0.5;
      
      double J_2beta = pow(J, -2 * beta);
      double coeff1 = 4 * beta * c1 * J_2beta;
      double coeff2 = 4 * c1 * J_2beta;
      
      apf::Matrix3x3 C_inv = apf::invert(C);
      
      for(int ii = 0; ii < 3; ii++)
	for(int jj = 0; jj < 3; jj++)
	  for(int kk = 0; kk < 3; kk++)
	    for(int ll = 0; ll < 3; ll++)
	      {
		// use the voigt matrix to convert indices from 4th-order tensor to 6x6 matrix
		int x = Voigt3x3[ii][jj];
		int y = Voigt3x3[kk][ll];
		
		strain[x][y] = 0.0;
		strain[x][y] += coeff1 * C_inv[ii][jj] * C_inv[kk][ll];
		strain[x][y] += coeff2 * 0.5 * 
		  (C_inv[ii][kk] * C_inv[jj][ll] + C_inv[ii][ll] * C_inv[jj][kk]);
	      }
    }

    void PK2Stress(const apf::Matrix3x3 & C,
		   double J,
		   double poisson_ratio,
		   double shear_modulus,
		   apf::Matrix3x3 & pk2)
    {
      double beta = poisson_ratio / ( 1 - 2 * poisson_ratio );
      double gamma1 = shear_modulus;
      double gamma2 = -shear_modulus * pow(J, -2 * beta);

      apf::Matrix3x3 C_inv = apf::invert(C);
      
      for(int ii = 0; ii < 3; ii++)
	for(int jj = 0; jj < 3; jj++)
	  pk2[ii][jj] = gamma1 * Identity3x3[ii][jj] + gamma2 * C_inv[ii][jj];
    }


    void PrintElementalSystem(std::ostream & stream,
			      int num_elemental_dofs,
			      const int * dof_numbers,
			      const double * Ke,
			      const double * fe)
    {
      int num_units = 3;
      for(int ii = 0; ii < num_elemental_dofs; ii++)
      {
	const int & global_num_i = dof_numbers[ii]; // row number
	for(int jj = 0; jj < num_elemental_dofs; jj++)
	{
	  stream << dof_numbers[jj] << " ";
	}
	stream << std::endl;
	
	if(Ke != NULL)
	{
	  stream << global_num_i << " ";
	  for(int jj = 0; jj < num_elemental_dofs; jj++)
	  {
	    const int & global_num_j = dof_numbers[jj]; // column number
	    const double & val = Ke[ii * num_elemental_dofs + jj];

	    stream << val << " ";
	  }
	  stream << std::endl;
	}
      }
      if(fe != NULL)
      {
	for(int jj = 0; jj < num_elemental_dofs; jj++)
	  stream << fe[jj] << std::endl;
      }
      
    }

    double* DynamicMatrix2Double(apf::DynamicMatrix & matrix)
    {
      int cols = matrix.getColumns();
      int rows = matrix.getRows();

      double * result = new double[cols*rows];
      for(int ii = 0; ii < cols; ii++)
	for(int jj = 0; jj < rows; jj++)
	  result[ii*cols + jj] = matrix(ii,jj);

      return result;
    }

    double* DynamicVector2Double(apf::DynamicVector& vector)
    {
      int size = vector.getSize();
      double * result = new double[size];
      for(int ii = 0; ii < size; ii++)
	result[ii] = vector(ii);
    }
  }
}
