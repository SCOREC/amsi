#include "simBoundaryConditions.h"
namespace amsi
{
  char const * bc_tps[] =
  {
    "dirichlet",
    "neumann"
  };
  char const * dir_bc_attrs[] =
  {
    "displacement"
  };
  char const * neu_bc_attrs[] =
  {
    "surface traction",
    "pressure"
  };
  char const * dis_bc_attrs[] =
  {
    "x",
    "y",
    "z"
  };
  char const * getBCTypeString(int tp)
  {
    assert(tp < NUM_BC_TYPES);
    return bc_tps[tp];
  }
  char const * getBCSubtypeString(int tp, int sbtp)
  {
    switch(tp)
    {
    case DIRICHLET:
      return getDirichletTypeString(sbtp);
    case NEUMANN:
      return getNeumannTypeString(sbtp);
    default:
      return NULL;
    }
  }
  char const * getDirichletTypeString(int tp)
  {
    assert(tp < NUM_DIRICHLET_TYPES);
    return dir_bc_attrs[tp];
  }
  char const * getNeumannTypeString(int tp)
  {
    assert(tp < NUM_NEUMANN_TYPES);
    return neu_bc_attrs[tp];
  }
    void applyBC(SimBC * bc, pMesh msh)
  {
    std::vector<pAttribute> atts;
    getBCAttributes(bc,std::back_inserter(atts));
    std::list<pEntity> ents;
    int dm = modelItemTypeDim(GEN_type((pGEntity)bc->itm));
    for(int d = dm; d >= 0; d--)
      getClassifiedEnts(msh,(pGEntity)bc->itm,d,std::back_inserter(ents));
  }
  void applyDirichletBC(SimBC * bc, pMesh msh)
  { }
  void processProblemDefinition(pACase ac)
  {
    //buildDirichletBCs(ac);
    // build/apply neumann bcs
  }
}
