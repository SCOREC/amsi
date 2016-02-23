#ifndef SIM_BOUNDARY_CONDITIONS_H_
#define SIM_BOUNDARY_CONDITIONS_H_
#include "amsiBoundaryConditions.h"
#include "amsiLAS.h"
#include "sim.h"
#include <apfNumbering.h>
#include <algorithm>
#include <cassert>
#include <cstring>
#include <iterator>
#include <vector>
namespace amsi
{
  /**
   * Get the enum value of a force boundary condition by c-string description.
   * @todo Eliminate the need for this
   */
  int getForceType(char const * nm);
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
  /**
   * A structure containing the necessary data to construct a SimBCQuery and get
   *  the mesh entities which the boundary condition is associated with.
   */
  struct SimBC
  {
    int tp;         // bc type (neumann/dirichlet)
    int sbtp;       // the bc subtype
    pANode bc_nd;   // the attribute node
    pModelItem itm; // the model entity
  };
  /**
   * A BCQuery class for all simmetrix-based boundary conditions to inherit from,
   *  provides access to the underlying SimBC structure from which the query is
   *  derived. Predominantly of use in querying for the associated mesh entities using
   *  getClassifiedEnts().
   */
  class SimBCQuery : public BCQuery
  {
  protected:
    SimBC * bc;
  public:
    SimBCQuery(SimBC * b)
      : BCQuery()
      , bc(b)
    { }
    SimBC * getSimBC() { return bc; }
  };
  /**
   * A BCQuery class specifc to the displacement dirichlet boundary condition.
   *  Seperate from a tensor1 query as each component of the field may be seperately
   *  fixed, and the number of fixed components varies.
   */
  class SimDisplacementQuery : public SimBCQuery
  {
  private:
    std::vector<pAttribute> atts;
  public:
    SimDisplacementQuery(SimBC * b);
    virtual int numComps();
    virtual bool isFixed(int ii);
    virtual bool isConst(int ii);
    virtual bool isTimeExpr(int ii);
    virtual bool isSpaceExpr(int ii);
    virtual double getValue(int ii, ...);
  };
  /**
   * A BCQuery wrapping a single simmetrix attribute,
   *  useful for most neumann boundary conditions
   *  which often only require a single attribute to specify.
   */
  class SimValueQuery : public SimBCQuery
  {
  protected:
    pAttribute att;
  public:
    SimValueQuery(SimBC * b);
    virtual bool isFixed(int ii);
    virtual bool isConst(int ii);
    virtual bool isTimeExpr(int ii);
    virtual bool isSpaceExpr(int ii);
  };
  /**
   * A specialization of the SimValueQuery wrapping a
   *  tensor0 or scalar value.
   */
  class SimTensor0Query : public SimValueQuery
  {
  public:
    SimTensor0Query(SimBC * b)
      : SimValueQuery(b)
    { };
    virtual int numComps() {return 1;}
    virtual double getValue(int ii, ...);
  };
  /**
   * A specialization of the SimValueQuery wrapping a
   *  tensor1 or vector value.
   */
  class SimTensor1Query : public SimValueQuery
  {
  public:
    SimTensor1Query(SimBC * b)
      : SimValueQuery(b)
    { }
    virtual int numComps() {return 3;}
    virtual double getValue(int ii, ...);
  };
  /**
   * Build the appropriate boundary condition query
   *  for the specified SimBC.
   */
  SimBCQuery * buildSimBCQuery(SimBC * bc);
  /**
   * Build the appropriate boundary condition queries
   *  for the specified boundary condition type, and specified
   *  boundary condition subtypes in range [bgn, nd).
   */
  template <typename I, typename O>
    void buildSimBCQueries(pACase pd, int tp, I bgn, I nd, O out);
  /**
   * Apply the SimBCQueries in range [bgn,nd) as dirichlet boundary conditions
   *  to the numbering and the field the numbering applies to for time t.
   */
  template <typename I>
    int applySimDirichletBCs(apf::Numbering * nm, pMesh msh, I bgn, I nd, double t);
  /**
   *
   */
  template <typename I>
    void applySimNeumannBCs(LAS * las, apf::Numbering * nm, pMesh msh, I bgn, I nd, double t);
  /**
   * Get the underlying simmetrix attributes relevant
   *  to the specific SimBC.
   */
  template <class O>
    void getBCAttributes(SimBC * bc, O out);
  /**
   * Build SimBC structures for boundary conditions of type tp,
   *  and subtypes in the range [begin,end).
   */
  template <class I, class O>
    void buildBCs(pACase ac, int tp, I begin, I end, O out);
}
#include "simBoundaryConditions_impl.h"
#endif
