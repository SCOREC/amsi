#include "FollowForceIntegrator.h"

#include <apfMesh.h>
#include <apfShape.h>

#include <cstring>

namespace amsi {
  namespace Analysis {

    FollowForceIntegrator::FollowForceIntegrator(apf::Mesh * msh, apf::Field * field, int o):
      NeumannIntegrator(field,o),
      msh(msh)
    {
      fs = apf::getShape(f);
    }

    void FollowForceIntegrator::atPoint(apf::Vector3 const & p, double w, double dV)
    {
      // get the shape functions
      apf::NewArray<double> N;
      apf::getShapeValues(e,p,N);

      apf::Vector3 xyz;
      apf::mapLocalToGlobal(me,p,xyz);

      apf::Vector3 normal;
      apf::MeshEntity * face;
      face = getMeshEntity(e);
      faceNormal(msh, face, normal);

      double x = xyz[0];
      double y = xyz[1];
      double z = xyz[2];

      double * F = tf->operator()(3,x,y,z);

      // Calculate following force by multiplying force specified in Simmodeler to face normal.
      F[0] = F[0]*normal.x();
      F[1] = F[1]*normal.y();
      F[2] = F[2]*normal.z();

      for(int ii = 0; ii < nenodes; ii++)
	for (int jj = 0; jj < num_field_components; jj++)
	  fe(ii*num_field_components + jj) = N[ii] * F[jj] * w * dV;
    }

  }
}
