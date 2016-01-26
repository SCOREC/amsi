#ifndef FOLLOWFORCEINTEGRATOR_H_
#define FOLLOWFORCEINTEGRATOR_H_

#include "NeumannIntegrator.h"
#include "TensorFieldQuery.h"
#include "apfFunctions.h"

namespace amsi {
  namespace Analysis {

    class FollowForceIntegrator : public NeumannIntegrator
    {
    public:
      FollowForceIntegrator(apf::Mesh * msh, apf::Field * field, int o);
      void atPoint(apf::Vector3 const &p, double w, double dV);

    private:
      apf::FieldShape * fs;
      apf::Mesh * msh;
    };
  }

}

#endif
