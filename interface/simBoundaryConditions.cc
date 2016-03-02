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
      if(strcmp(itm,arr[ii]) == 0)
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
    assert(tp < NUM_DIRICHLET_TYPES && tp >= 0);
    return dir_bc_attrs[tp];
  }
  char const * getNeumannTypeString(int tp)
  {
    assert(tp < NUM_NEUMANN_TYPES && tp >= 0);
    return neu_bc_attrs[tp];
  }
  SimBCQuery * buildSimDirichletBCQuery(SimBC * bc)
  {
    switch(bc->sbtp)
    {
    case DISPLACEMENT:
      return new SimDisplacementQuery(bc);
    default:
      return NULL;
    }
  }
  SimBCQuery * buildSimNeumannBCQuery(SimBC * bc)
  {
    switch(bc->sbtp)
    {
    case SURFACE_TRACTION:
      return new SimTensor1Query(bc);
    case NORMAL_PRESSURE:
      return new SimTensor0Query(bc);
    default:
      return NULL;
    }
  }
  SimBCQuery * buildSimBCQuery(SimBC * bc)
  {
    if(bc->tp == DIRICHLET)
      return buildSimDirichletBCQuery(bc);
    else if(bc->tp == NEUMANN)
      return buildSimNeumannBCQuery(bc);
    return NULL;
  }
  SimDisplacementQuery::SimDisplacementQuery(SimBC * b)
    : SimBCQuery(b)
    , atts()
  {
    atts.assign(numDirichletComponents(bc->sbtp),(pAttribute)NULL);
    std::vector<pAttribute> rw;
    getBCAttributes(bc,std::back_inserter(rw));
    auto nd = rw.end();
    for(auto att = rw.begin(); att != nd; ++att)
    {
      char * att_tp = Attribute_infoType(*att);
      atts[findAttrIndex(dis_bc_attrs,3,att_tp)] = *att;
      Sim_deleteString(att_tp);
    }
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
    if(isConst(ii))
      return AttributeTensor0_value((pAttributeTensor0)atts[ii]);
    double rslt = 0.0;
    int arg_cnt = isTimeExpr(ii) ? 1 : 0;
    arg_cnt += isSpaceExpr(ii) ? 3 : 0;
    va_list prms;
    va_start(prms,ii);
    double * args = new double[arg_cnt];
    for(int ii = 0; ii < arg_cnt; ii++)
      args[ii] = va_arg(prms, double);
    if(isTimeExpr(ii))
      rslt = AttributeTensor0_evalDT((pAttributeTensor0)atts[ii],args[0]);
    else if(isSpaceExpr(ii))
      rslt = AttributeTensor0_evalDS((pAttributeTensor0)atts[ii],&args[0]);
    va_end(prms);
    delete [] args;
    return rslt;
  }
  SimValueQuery::SimValueQuery(SimBC * b)
    : SimBCQuery(b)
    , att()
  {
    getBCAttributes(bc,&att);
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
    if(isConst(ii))
      return AttributeTensor0_value((pAttributeTensor0)att);
    double rslt = 0.0;
    int arg_cnt = isTimeExpr(ii) ? 1 : 0;
    arg_cnt += isSpaceExpr(ii) ? 3 : 0;
    double * args = new double[arg_cnt];
    va_list prms;
    va_start(prms,ii);
    for(int ii = 0; ii < arg_cnt; ii++)
      args[ii] = va_arg(prms,double);
    if(isTimeExpr(ii))
      rslt = AttributeTensor0_evalDT((pAttributeTensor0)att,args[0]);
    else if(isSpaceExpr(ii))
      rslt = AttributeTensor0_evalDS((pAttributeTensor0)att,&args[0]);
    va_end(prms);
    delete args;
    return rslt;
  }
  double SimTensor1Query::getValue(int ii, ...)
  {
    if(isConst(ii))
      return AttributeTensor1_value((pAttributeTensor1)att,ii);
    double rslt = 0.0;
    int arg_cnt = isTimeExpr(ii) ? 1 : 0;
    arg_cnt += isSpaceExpr(ii) ? 3 : 0;
    double * args = new double[arg_cnt];
    va_list prms;
    va_start(prms,ii);
    for(int ii = 0; ii < arg_cnt; ii++)
      args[ii] = va_arg(prms,double);
    if(isTimeExpr(ii))
      rslt = AttributeTensor1_evalDT((pAttributeTensor1)att,ii,args[0]);
    else if(isSpaceExpr(ii))
      rslt = AttributeTensor1_evalDS((pAttributeTensor1)att,ii,&args[0]);
    va_end(prms);
    delete [] args;
    return rslt;
  }
}
