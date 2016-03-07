#ifndef AMSI_BOUNDARY_CONDITIONS_H_
#define AMSI_BOUNDARY_CONDITIONS_H_
namespace amsi
{
  enum BCTypes
  {
    DIRICHLET,
    NEUMANN,
    NUM_BC_TYPES
  };
  enum NeumannBCType
  {
    SURFACE_TRACTION,
    NORMAL_PRESSURE,
    NUM_NEUMANN_TYPES
  };
  int numBCComponents(int tp, int sbtp);
  int numDirichletComponents(int tp);
  int numNeumannComponents(int tp);
  class BCQuery
  {
  public:
    virtual int numComps() = 0;
    virtual bool isFixed(int ii = 0) = 0;
    virtual bool isConst(int ii = 0) = 0;
    virtual bool isTimeExpr(int ii = 0) = 0;
    virtual bool isSpaceExpr(int ii = 0) = 0;
    virtual double getValue(int ii = 0, ...) = 0;
  };
  class BCQueryBuilder
  {
  public:
    BCQuery * buildQuery();
  };
  template <typename O>
    void getApplicableBCTypesForField(int fld_tp, int bc_tp, O out);
}
#include "amsiBoundaryConditions_impl.h"
#endif
