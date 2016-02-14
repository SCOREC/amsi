#ifndef NEUMANNINTEGRATOR_H_
#define NEUMANNINTEGRATOR_H_
#include "TensorFieldQuery.h"
#include <apf.h>
#include <apfDynamicVector.h>
namespace amsi
{
  namespace Analysis
  {
    class NeumannIntegrator : public apf::Integrator
    {
    protected:
      apf::DynamicVector fe;
      apf::MeshElement * me;
      apf::Element * e;
      apf::Field * f;
      int nedofs;
      int nenodes;
      int num_field_components;
      TensorFieldQuery * tf;
    public:
      NeumannIntegrator(apf::Field * field, int o);
      void setTensorQuery(TensorFieldQuery * q) {delete tf; tf = q;}
      virtual void inElement(apf::MeshElement *);
      virtual void outElement() {};
      virtual void parallelReduce() {};
      virtual int numDofs() {return nedofs;}
      virtual apf::DynamicVector& getFe() {return fe;}
    };
  }
}
#endif
