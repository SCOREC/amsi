#include "simAttributes.h"
#include <cassert> //move to _impl.h
namespace amsi
{
  template <typename O>
    void getAnalysisCases(pGModel mdl, O out)
  {
    amsi::getTypeCases(SModel_attManager(mdl),"analysis",out);
  }
  template <typename O>
    void getClassifiedEnts(pMesh msh, pGEntity mdl_ent, int dm, O out)
  {
    switch(dm)
    {
    case 3:
    {
      RIter it = M_classifiedRegionIter(msh,mdl_ent);
      pRegion pr;
      while ((pr = RIter_next(it)))
        *out++ = (pEntity)pr;
      RIter_delete(it);
      break;
    }
    case 2:
    {
      FIter it = M_classifiedFaceIter(msh,mdl_ent,1);
      pFace pf;
      while ((pf = FIter_next(it)))
        *out++ = (pEntity)pf;
      FIter_delete(it);
      break;
    }
    case 1:
    {
      EIter it = M_classifiedEdgeIter(msh,mdl_ent,1);
      pEdge pe;
      while ((pe = EIter_next(it)))
        *out++ = (pEntity)pe;
      EIter_delete(it);
      break;
    }
    case 0:
    {
      VIter it = M_classifiedVertexIter(msh,mdl_ent,1);
      pVertex pv;
      while ((pv = VIter_next(it)))
        *out++ = (pEntity)pv;
      VIter_delete(it);
      break;
    }
    }
  }
  template <typename O>
    void getClassifiedDimEnts(pMesh msh, pGEntity mdl_ent, int ld, int hd, O out)
  {
    assert(hd >= ld);
    for(int d = ld; d <= hd; d++)
      getClassifiedEnts(msh,mdl_ent,d,out);
  }
}
