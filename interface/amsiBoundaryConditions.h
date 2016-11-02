#ifndef AMSI_BOUNDARY_CONDITIONS_H_
#define AMSI_BOUNDARY_CONDITIONS_H_
namespace amsi
{
  enum BCTypes
  {
    DIRICHLET = 0,
    NEUMANN = 1,
    NUM_BC_TYPES = 2
  };
  enum NeumannBCType
  {
    CUSTOM = 0,
    SURFACE_TRACTION = 1,
    NORMAL_PRESSURE = 2,
    NUM_NEUMANN_TYPES = 3
  };
  int numBCComponents(int tp, int sbtp);
  int numDirichletComponents(int tp);
  int numNeumannComponents(int tp);
  struct BC
  {
    int tp;
    int sbtp;
  };
  class BCQuery
  {
  protected:
    BC * bc;
  public:
    BCQuery(BC * b)
      : bc(b)
    { }
    virtual int numComps()
    {
      return numBCComponents(bc->tp,bc->sbtp);
    }
    virtual bool isFixed(int ii = 0) = 0;
    virtual bool isConst(int ii = 0) = 0;
    virtual bool isTimeExpr(int ii = 0) = 0;
    virtual bool isSpaceExpr(int ii = 0) = 0;
    virtual double getValue(int ii = 0, ...) = 0;
  };
  /**
   * Get a c-style string describing the boundary condition type;
   */
  char const * getBCTypeString(int tp);
  /**
   * Get a c-style string describing the type of boundary condition's subtype
   */
  char const * getBCSubtypeString(int tp, int sbpt);
  /**
   * Get a c-style string describing the dirichlet boundary condition type.
   */
  char const * getDirichletTypeString(int tp);
  /**
   * Get a c-style string describing the neumann boundary condition type.
   */
  char const * getNeumannTypeString(int tp);
  template <typename O>
    void getApplicableBCTypesForField(int fld_tp, int bc_tp, O out);
}
#include "amsiBoundaryConditions_impl.h"
#endif
