#ifndef NONLINEARELASTICINTEGRATOR_H_
#define NONLINEARELASTICINTEGRATOR_H_

#include <apf.h>
#include <apfDynamicMatrix.h>
#include <apfNumbering.h>
#include <apfShape.h>


namespace amsi {
  namespace Analysis {
    class NonLinearElasticIntegrator : public apf::Integrator
    {
    public:
      NonLinearElasticIntegrator(apf::Field * field,
				 int o,
				 double poisson_ratio,
				 double shear_modulus);
    
      
      void inElement(apf::MeshElement * melm);
      void outElement(apf::MeshElement * melm) {};
      void atPoint(apf::Vector3 const &p, double w, double dV);

      void parallelReduce() {};

      int numElementalEqs() {return num_element_eqs;}
      apf::DynamicMatrix& Ke() {return ke;}
      apf::DynamicVector& Fe() {return fe;}
      apf::Element * E() {return e;}
      apf::NewArray<apf::Vector3>& NodeValues() {return node_values;}
    private:
      apf::DynamicMatrix ke;
      apf::DynamicVector fe;
      apf::Element * e;
      apf::Field * f;
      apf::FieldShape * fs;
      apf::MeshElement * me;

      apf::NewArray<apf::Vector3> node_values;

      double poisson_ratio;
      double shear_modulus;
      
      int num_element_eqs;
      int num_field_components;
      int num_element_nodes;
    };
  }
}

#endif
