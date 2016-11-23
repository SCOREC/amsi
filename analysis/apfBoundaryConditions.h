#ifndef AMSI_APF_BOUNDARY_CONDITIONS_H_
#define AMSI_APF_BOUNDARY_CONDITIONS_H_
#include "apfMesh.h"
namespace amsi
{
  class NeumannIntegrator;
  class BCQuery;
  class LAS;
  double getDirichletValue(BCQuery * qry,
                           apf::Mesh * msh,
                           apf::MeshEntity * ent,
                           int nd,
                           int cmp,
                           double t);
  /*
  double getNeumannValue(BCQuery * qry,
                         apf::Mesh * msh,
                         apf::MeshEntity * ent,
                         int nd,
                         int cmp,
                         double t);
  */
  template <typename I>
    int applyDirichletBC(apf::Numbering * nm,
                         I begin,
                         I end,
                         BCQuery * qry,
                         double t);
  template <typename I>
    void applyNeumannBC(LAS * las,
                        apf::Numbering * nm,
                        I bgn,
                        I nd,
                        NeumannIntegrator * i,
                        double t);
}
#include "apfBoundaryConditions_impl.h"
#endif
