#include "NeumannIntegrator.h"

namespace amsi
{
  namespace Analysis
  {

    NeumannIntegrator::NeumannIntegrator(apf::Field * field,int o) :
      apf::Integrator(o),
      fe(),
      me(NULL),
      e(NULL),
      f(field),
      nedofs(0),
      nenodes(0),
      num_field_components(0),
      tf(NULL)
    {}
    
    void NeumannIntegrator::inElement(apf::MeshElement * ME)
    {
      me = ME;
      e = apf::createElement(f,me);
      nenodes = apf::countNodes(e);
      num_field_components = apf::countComponents(f);
      int new_nedofs = nenodes * num_field_components;
      bool reallocate = new_nedofs != nedofs;
      nedofs = new_nedofs;
      if(reallocate)
        fe.setSize(nedofs);
      fe.zero();
    }
  }
}
