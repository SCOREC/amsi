#include "NonFiniteElement.h"
namespace amsi
{
  void linearStiffnessK0(int num_nodes,
                         apf::DynamicMatrix & B0,
                         apf::Matrix<6,6> & hookes,
                         apf::DynamicMatrix & K0)
  {
    apf::DynamicMatrix B0T;
    apf::transpose(B0,B0T);
    apf::DynamicMatrix T(6,6);
    T.zero();
    for(int ii = 0; ii < num_nodes; ii++)
      for(int jj = 0; jj < 6; jj++)
        for(int kk = 0; kk < 6; kk++)
          T(ii,jj) += B0T(ii,kk) * hookes[kk][jj];
    apf::multiply(T,B0,K0);
  }
  void nonLinearStiffness(int,
                          apf::DynamicMatrix & BNL,
                          apf::Matrix3x3 & pk2,
                          apf::DynamicMatrix & KNL)
  {
    apf::DynamicMatrix BNLT; //12x9 num_nodes * 3 (dofs_per_node) x 9
    apf::transpose(BNL,BNLT);
    apf::DynamicMatrix expanded_pk2(9,9);
    expanded_pk2.zero();
    for(int ii = 0; ii < 3; ii++)
      for(int jj = 0; jj < 3; jj++)
      {
        expanded_pk2(ii,jj)     = pk2[ii][jj];
        expanded_pk2(ii+3,jj+3) = pk2[ii][jj];
        expanded_pk2(ii+6,jj+6) = pk2[ii][jj];
      }
    KNL.setSize(BNLT.getRows(), BNL.getColumns());
    apf::DynamicMatrix T;
    apf::multiply(BNLT,expanded_pk2,T);
    apf::multiply(T,BNL,KNL);
  }
  void linearStrainDispB0(int num_nodes,
                          apf::NewArray<apf::Vector3> & shape_deriv,
                          apf::DynamicMatrix & B)
  {
    B.setSize(6,3*num_nodes);
    B.zero();
    for(int ii = 0; ii < num_nodes; ii++)
    {
      apf::Vector3 disp = shape_deriv[ii];
      B(0,3*ii + 0) = disp[0];
      B(1,3*ii + 1) = disp[1];
      B(2,3*ii + 2) = disp[2];
      B(3,3*ii + 0) = disp[1];
      B(3,3*ii + 1) = disp[0];
      B(4,3*ii + 1) = disp[2];
      B(4,3*ii + 2) = disp[1];
      B(5,3*ii + 0) = disp[2];
      B(5,3*ii + 2) = disp[0];
    }
  }
  void computeGreenStrain(apf::DynamicMatrix & ,
                          apf::DynamicMatrix & ,
                          apf::Matrix3x3 & )
  {
    /*
      double L[3][3];
      for(int i=0; i<3; i++)
      {
      for(int j=0; j<3; j++)
      {
      L[i][j]=0.0;
      for(int k=0; k<4; k++)
      {
      L[i][j]+=H[k][j]*u[3*k+i];
      }
      }
      }
      double temp;
      for(int i=0; i<3; i++)
      {
      for(int j=0; j<3; j++)
      {
      temp = 0.0;
      for(int k=0; k<3; k++) {
      temp += L[k][i]*L[k][j];
      }
      E[i][j] =  0.5*(L[i][j]+L[j][i]+temp);
      }
      }
    */
  }
  void linearStrainDispB1(int num_nodes,
                          apf::NewArray<apf::Vector3> & shape_deriv,
                          apf::NewArray<apf::Vector3> & node_disp,
                          apf::DynamicMatrix & B)
  {
    B.setSize(6,num_nodes*3);
    B.zero();
    apf::Matrix3x3 L;
    for(int kk = 0; kk < num_nodes; kk++)
      for(int ii = 0; ii < 3; ii++)
        for(int jj = 0; jj < 3; jj++)
          L[ii][jj] += shape_deriv[kk][jj] * node_disp[kk][ii];
    double ind_one[] = {0,1,0};
    double ind_two[] = {0,2,2};
    for(int ii = 0; ii < num_nodes; ii++)
    {
      for(int jj = 0; jj < 3; jj++)
      {
        B(jj,3*ii+0) = L[0][jj] * shape_deriv[ii][jj];
        B(jj,3*ii+1) = L[1][jj] * shape_deriv[ii][jj];
        B(jj,3*ii+2) = L[2][jj] * shape_deriv[ii][jj];
      }
      for(int jj = 0; jj < 3; jj++)
      {
        B(3+jj,3*ii+0) = L[0][ind_one[jj]] * shape_deriv[ii][ind_two[jj]]
          + L[0][ind_two[jj]] * shape_deriv[ii][ind_one[jj]];

        B(3+jj,3*ii+1) = L[1][ind_one[jj]] * shape_deriv[ii][ind_two[jj]]
          + L[1][ind_two[jj]] * shape_deriv[ii][ind_one[jj]];

        B(3+jj,3*ii+2) = L[2][ind_one[jj]] * shape_deriv[ii][ind_two[jj]]
          + L[2][ind_two[jj]] * shape_deriv[ii][ind_one[jj]];
      }
    }
  }
  void linearStrainDisp(int num_nodes,
                        apf::NewArray<apf::Vector3> & shape_deriv,
                        apf::NewArray<apf::Vector3> & node_disp,
                        apf::DynamicMatrix & BL)
  {
    apf::DynamicMatrix B0, B1;
    linearStrainDispB0(num_nodes,shape_deriv,B0);
    linearStrainDispB1(num_nodes,shape_deriv,node_disp,B1);
    BL.setSize(B1.getRows(),B1.getColumns());
    BL.zero();
    BL += B0;
    BL += B1;
  }
  void nonLinearStrainDisp(int num_nodes,
                           apf::NewArray<apf::Vector3> & shape_deriv,
                           apf::DynamicMatrix & B)
  {
    B.setSize(9,3*num_nodes);
    B.zero();
    for(int jj = 0; jj < num_nodes; jj++)
    {
      B(0,3*jj+0) = B(3,3*jj+0) = B(6,3*jj+0) = shape_deriv[jj][0];
      B(1,3*jj+1) = B(4,3*jj+1) = B(5,3*jj+1) = shape_deriv[jj][1];
      B(6,3*jj+2) = B(7,3*jj+2) = B(8,3*jj+2) = shape_deriv[jj][2];
    }
  }
}
















