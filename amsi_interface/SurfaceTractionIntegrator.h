#ifndef SURFACETRACTIONINTEGRATOR_H_
#define SURFACETRACTIONINTEGRATOR_H_

#include "NeumannIntegrator.h"
#include "TensorFieldQuery.h"

namespace amsi {
  namespace Analysis {

    class SurfaceTractionIntegrator : public NeumannIntegrator
    {
    public: 
      SurfaceTractionIntegrator(apf::Field * field,
				int o);
      void atPoint(apf::Vector3 const &p, double w, double dV);
    private:
      apf::FieldShape * fs;
    };
  }

}

#endif
