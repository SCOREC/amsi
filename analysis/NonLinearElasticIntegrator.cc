#include "NonLinearElasticIntegrator.h"
#include "apfFunctions.h"
#include "ConvenienceFunctions.h"
#include "NonFiniteElement.h"
#include <apfMesh.h>
#include <apfShape.h>
namespace amsi
{
  NonLinearElasticIntegrator::NonLinearElasticIntegrator(apf::Field * field,
                                                         int o,
                                                         double pr,
                                                         double sm)
    : apf::Integrator(o)
    , ke()
    , fe()
    , e(NULL)
    , f(field)
    , fs(NULL)
    , me(NULL)
    , node_values()
    , poisson_ratio(pr)
    , shear_modulus(sm)
    , num_element_eqs(0)
    , num_field_components(0)
    , num_element_nodes(0)
  {
    fs = apf::getShape(f);
    num_field_components = apf::countComponents(f);
  }
  void NonLinearElasticIntegrator::inElement(apf::MeshElement * melm)
  {
    me = melm;
    e = apf::createElement(f,me);
    num_element_nodes = apf::countNodes(e);
    int new_num_element_eqs = num_element_nodes * num_field_components;
    bool reallocate = new_num_element_eqs != num_element_eqs;
    num_element_eqs = new_num_element_eqs;
    apf::getVectorNodes(e,node_values);
    if(reallocate)
    {
      ke.setSize(num_element_eqs,num_element_eqs);
      fe.setSize(num_element_eqs);
    }
    ke *= 0.0;
    fe.zero();
  }
  void NonLinearElasticIntegrator::atPoint(apf::Vector3 const &p, double w, double dV)
  {
    apf::Matrix3x3 J;
    double wxdetjac = w * dV;
    apf::NewArray<apf::Vector3> shape_derivs;
    apf::getShapeGrads(e,p,shape_derivs);
    apf::Matrix3x3 grad;
    double detgrad;
    calcDefGrad(e,p,grad,detgrad);
    apf::Matrix3x3 C;
    RightCauchy(grad,C);
    apf::Matrix<6,6> hookes;
    LinearizedConstitutive(C,dV,poisson_ratio,shear_modulus,hookes);
    apf::Matrix3x3 pk2;
    PK2Stress(C,dV,poisson_ratio,shear_modulus,pk2);
    apf::DynamicMatrix BL;
    linearStrainDisp(num_element_nodes,
                     shape_derivs,
                     node_values,
                     BL);
    apf::DynamicMatrix BNL;
    nonLinearStrainDisp(num_element_nodes,
                        shape_derivs,
                        BNL);
    apf::DynamicMatrix K0;
    linearStiffnessK0(num_element_nodes,
                      BL,
                      hookes,
                      K0);
    apf::DynamicMatrix KNL;
    nonLinearStiffness(num_element_nodes,
                       BNL,
                       pk2,
                       KNL);
    ke += K0;
    ke += KNL;
    ke *= wxdetjac;
    apf::Vector<6> pk2_vec;
    SymmMatrixToVoigtVector(pk2,pk2_vec);
    for(int ii = 0; ii < num_element_eqs; ii++)
      for(int jj = 0; jj < num_element_eqs; jj++)
        fe(ii) += BL(jj,ii) * pk2_vec[jj];
    fe *= (-wxdetjac);
  }
}
