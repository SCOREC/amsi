#include "apfsimWrapper.h"
#include "amsiMPI.h"
#include "apfFunctions.h"
#include "simAnalysis.h"
namespace amsi
{
  void applyUniqueRegionTags(pGModel mdl, pMesh msh, apf::Mesh * apfmsh)
  {
    pGEntity rgn = NULL;
    apf::Field * fld = apf::createIPField(apfmsh,"region_id",apf::SCALAR,1);
    int rgn_tg = 0;
    auto grit = GM_regionIter(mdl);
    while((rgn = GRIter_next(grit)))
    {
      std::list<pEntity> rgn_ents;
      getClassifiedEnts(msh,rgn,3,std::back_inserter(rgn_ents));
      rgn_tg = GEN_tag(rgn);
      setEntitiesNode(fld,rgn_tg,rgn_ents.begin(),rgn_ents.end());
    }
    GRIter_delete(grit);
  }
}
