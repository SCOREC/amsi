#include "simBoundaryConditions.h"
#include <cstdarg>
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
  int findAttrIndex(char const * arr[], int cnt, char const * itm)
  {
    for(int ii = 0; ii < cnt; ii++)
      if(strcmp(itm,arr[ii]))
        return ii;
    return -1;
  }
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
  BCQuery * buildSimDirichletBCQuery(SimBC * bc)
  {
    switch(bc->sbtp)
    {
    case DISPLACEMENT:
      return new SimDisplacementQuery(bc);
    default:
      return NULL;
    }
  }
  BCQuery * buildSimNeumannBCQuery(SimBC * bc)
  {
    switch(bc->sbtp)
    {
    case SURFACE_TRACTION:
      return new SimTensor1Query(bc);
    case PRESSURE:
      return new SimTensor0Query(bc);
    default:
      return NULL;
    }
  }
  BCQuery * buildSimBCQuery(SimBC * bc)
  {
    if(bc->tp == DIRICHLET)
      return buildSimDirichletBCQuery(bc);
    else if(bc->tp == NEUMANN)
      return buildSimNeumannBCQuery(bc);
    return NULL;
  }
  SimDisplacementQuery::SimDisplacementQuery(SimBC * b)
    : bc(b)
    , atts()
  {
    atts.assign(numDirichletComponents(bc->sbtp),NULL);
    std::vector<pAttribute> rw;
    getBCAttributes(bc,std::back_inserter(rw));
    for(auto att : rw)
      atts[findAttrIndex(dis_bc_attrs,3,Attribute_infoType(att))] = att;
  }
  int SimDisplacementQuery::numComps()
  {
    return numDirichletComponents(bc->sbtp);
  }
  bool SimDisplacementQuery::isFixed(int ii)
  {
    return atts[ii] != NULL;
  }
  bool SimDisplacementQuery::isConst(int ii)
  {
    assert(isFixed(ii));
    return AttributeTensor0_constant((pAttributeTensor0)atts[ii]);
  }
  bool SimDisplacementQuery::isTimeExpr(int ii)
  {
    return ! isConst(ii);
  }
  bool SimDisplacementQuery::isSpaceExpr(int)
  {
    return false;
  }
  double SimDisplacementQuery::getValue(int ii, ...)
  {
    assert(isFixed(ii));
    double rslt = 0.0;
    int arg_cnt = isTimeExpr(ii) ? 1 : 0;
    arg_cnt += isSpaceExpr(ii) ? 3 : 0;
    va_list prms;
    va_start(prms,ii);
    double args[arg_cnt] = {};
    for(int ii = 0; ii < arg_cnt; ii++)
      args[ii] = va_arg(prms, double);
    if(isTimeExpr(ii))
      rslt = AttributeTensor0_evalDT((pAttributeTensor0)atts[ii],args[0]);
    else if(isSpaceExpr(ii))
      rslt = AttributeTensor0_evalDS((pAttributeTensor0)atts[ii],&args[0]);
    va_end(prms);
    return rslt;
  }
  SimValueQuery::SimValueQuery(SimBC * b)
    : bc(b)
    , att()
  {
    getBCAttributes(bc,&att);
  }
  int SimValueQuery::numComps()
  {
    return numNeumannComponents(bc->tp);
  }
  bool SimValueQuery::isFixed(int)
  {
    return false;
  }
  bool SimValueQuery::isConst(int)
  {
    return isAttrConst(att);
  }
  bool SimValueQuery::isTimeExpr(int ii)
  {
    return ! isConst(ii);
  }
  bool SimValueQuery::isSpaceExpr(int)
  {
    return false;
  }
  double SimTensor0Query::getValue(int ii, ...)
  {
    return 0.0;
  }
  double SimTensor1Query::getValue(int ii, ...)
  {
    return 0.0;
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
}
