#include "NeumannIntegrator.h"

namespace amsi
{
  namespace Analysis
  {

    NeumannIntegrator::NeumannIntegrator(apf::Field * field,int o) :
      apf::Integrator(o),
      f(field),
      fe(),
      e()
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
