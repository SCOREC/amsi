#ifndef TRUSSINTEGRATOR_H_
#define TRUSSINTEGRATOR_H_

#include <apf.h>
#include <apfDynamicMatrix.h>
#include <apfMesh.h>

namespace amsi {
  namespace Analysis {

    class TrussIntegrator : public apf::Integrator
    {
    public:
      TrussIntegrator(apf::Field * field,
		      int o,
		      double a,
		      double b);

      void inElement(apf::MeshElement * melm);
      void outElement(apf::MeshElement * melm) {};
      void atPoint(apf::Vector3 const & p, double w, double dV);

      void parallelReduce() {};

      apf::DynamicMatrix& Ke() {return ke;}
      apf::DynamicVector& Fe() {return fe;}
      apf::NewArray<apf::Vector3>& NodeValues() {return node_values;}
      apf::Element * E() {return e;}
    private:
      apf::DynamicMatrix ke;
      apf::DynamicVector fe;
      apf::Mesh * mesh;
      apf::Field * f;

      apf::Element * e;
      apf::MeshElement * me;
      
      apf::NewArray<apf::Vector3> node_values;
      apf::Vector3 xyz[2];

      int num_element_eqs;
      int num_field_components;
      int num_element_nodes;

      double A, B;
    };
    
  }
}

#endif
