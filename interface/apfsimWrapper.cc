#include "apfsimWrapper.h"
#include "apfFunctions.h"
#include "simAnalysis.h"
namespace amsi
{
  double measureEntity(pGEntity ent, pMesh msh, apf::Mesh * apf_msh)
  {
    // todo : determine dimension of entity
    double vol = 0.0;
    std::list<pEntity> ents;
    getClassifiedEnts(msh,ent,3,std::back_inserter(ents));
    for(std::list<pEntity>::iterator ent = ents.begin(); ent != ents.end(); ++ent)
    {
      apf::MeshElement * mnt = apf::createMeshElement(apf_msh,apf::castEntity(*ent));
      vol += apf::measure(mnt);
      apf::destroyMeshElement(mnt);
    }
    vol = amsi::comm_sum(vol);
    return vol;
  }
  double measureDisplacedEntity(pGEntity ent, pMesh msh, apf::Field * u)
  {
    double vol = 0.0;
    std::list<pEntity> ents;
    getClassifiedEnts(msh,ent,3,std::back_inserter(ents));
    for(std::list<pEntity>::iterator ent = ents.begin(); ent != ents.end(); ++ent)
      vol += measureDisplaced(apf::castEntity(*ent),u);
    vol = amsi::comm_sum(vol);
    return vol;
  }
  void applyUniqueRegionTags(pGModel mdl, pMesh msh, apf::Mesh * apfmsh)
  {
    pGEntity rgn = NULL;
    apf::Field * fld = apf::createIPField(apfmsh,"region_id",apf::SCALAR,1);
    int rgn_tg = 0;
    for(auto grit = GM_regionIter(mdl); (rgn = GRIter_next(grit)); )
    {
      std::list<pEntity> rgn_ents;
      getClassifiedEnts(msh,rgn,3,std::back_inserter(rgn_ents));
      rgn_tg = GEN_tag(rgn);
      setEntitiesNode(fld,rgn_tg,rgn_ents.begin(),rgn_ents.end());
    }
  }
}
