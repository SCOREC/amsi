#ifndef AMSI_APF_BOUNDARY_CONDITIONS_H_
#define AMSI_APF_BOUNDARY_CONDITIONS_H_
#include "amsiBoundaryConditions.h"
#include "apfMesh.h"
namespace amsi
{
  double getDirichletValue(BCQuery * qry,
                           apf::Mesh * msh,
                           apf::MeshEntity * ent,
                           int nd,
                           int cmp,
                           double t);
  template <typename T>
    int applyDirichletBC(apf::Numbering * nm,
                         T begin,
                         T end,
                         BCQuery * qry,
                         double t);
}
#include "apfBoundaryConditions_impl.h"
#endif
