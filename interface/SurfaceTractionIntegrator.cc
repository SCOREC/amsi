#include "SurfaceTractionIntegrator.h"

#include <apfMesh.h>
#include <apfShape.h>

#include <cstring> //memset

namespace amsi {
  namespace Analysis {

    SurfaceTractionIntegrator::SurfaceTractionIntegrator(apf::Field * field,
                                                         int o) :
      NeumannIntegrator(field,o)
    {
      fs = apf::getShape(f);
    }
    void SurfaceTractionIntegrator::atPoint(apf::Vector3 const &p, double w, double dV)
    {
      // get the shape functions
      apf::NewArray<double> N;
      apf::getShapeValues(e,p,N);

      apf::Vector3 xyz;
      apf::mapLocalToGlobal(me,p,xyz);

      double x = xyz[0];
      double y = xyz[1];
      double z = xyz[2];

      double * F = tf->operator()(3,x,y,z);

      for(int ii = 0; ii < nenodes; ii++)
        for(int jj = 0; jj < num_field_components; jj++)
          fe(ii*num_field_components + jj) = N[ii] * F[jj] * w * dV;
    }

  }
}
