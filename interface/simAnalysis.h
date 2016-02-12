#ifndef AMSI_SIM_ANALYSIS_H_
#define AMSI_SIM_ANALYSIS_H_
#include "sim.h"
#include "simAttributes.h"
namespace amsi
{
  /// TODO: make this a wrapper and create a getClassifiedDimEnts,,,?
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
  struct SimAnalysis
  {
    pGModel mdl;
    pACase cs;
  SimAnalysis(pGModel m, pACase c)
  : mdl(m)
  , cs(c)
      { }
  };
  struct SimFEA
  {
    SimAnalysis * an;
    pParMesh msh;
  SimFEA(pParMesh m, SimAnalysis * a)
  : an(a)
  , msh(m)
      { }
  };
  // an analysis, related to a single "analysis" case of a single model
  template <typename I, typename O>
    void buildSimAnalyses(const I & nms, O out)
  {
    for(auto nm : nms)
    {
      pGModel mdl = GM_load(nm,0,NULL);
      pAManager attm = SModel_attManager(mdl);
      std::vector<pACase> css;
      getTypeCases(attm,"analysis",std::back_inserter(css));
      for(auto cs : css)
        *out++ = new SimAnalysis(mdl,cs);
    }
  }
}
#endif
