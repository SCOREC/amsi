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
  enum DirichletBCType
  {
    DISPLACEMENT,
    NUM_DIRICHLET_TYPES
  };
  enum NeumannBCType
  {
    SURFACE_TRACTION,
    PRESSURE,
    NUM_NEUMANN_TYPES
  };
  int numDirichletComponents(int tp);
  class DirichletSpec
  {
  public:
    virtual int numComps() = 0;
    virtual bool isFixed(int ii) = 0;
    virtual bool isConst(int ii) = 0;
    virtual bool isTimeExpr(int ii) = 0;
    virtual bool isSpaceExpr(int ii) = 0;
    virtual double getValue(int ii, ...) = 0;
  };
}
#endif
