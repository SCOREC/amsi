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
  double measureDisplacedEntityFromSurf(pGEntity ent, pMesh msh, apf::Field * u)
  {
    /**
     * pGEntity ent:   face entity of geometric model.
     * pMesh msh:      partitioned mesh.
     * apf::Field * u: displacement field. */
    double vol = 0.0;
    /** Extract face entities that are adjacent to region entity of geometric model */
    pPList adjpGFaces = GR_faces((pGRegion)ent);
    std::cout<<"number of adjacent faces is "<<PList_size(adjpGFaces)<<std::endl;
    /** Extract face entities of mesh that lie on face entities of geometric model */
    std::list<pEntity> ents;
    std::vector<int> tags;
    void* iter = 0;
    void* pGF;
    int ent_size = 0;
    while (pGF = PList_next(adjpGFaces,&iter))
    {
      getClassifiedEnts(msh,(pGFace)pGF,2,std::back_inserter(ents));
      for (int ii=ent_size; ii<ents.size(); ii++)
      {
	tags.push_back(GEN_tag((pGFace)pGF));
	ent_size = ents.size();
      }
    }
    PList_delete(adjpGFaces);

//    getClassifiedEnts(msh,ent,2,std::back_inserter(ents));
    int surf_elem = 0;
    for(std::list<pEntity>::iterator ent = ents.begin(); ent != ents.end(); ++ent)
    {
//      std::cout<<"surf_elem:"<<surf_elem<<" belongs to face "<<tags[surf_elem]
//	       <<std::endl;
      vol += measureDisplacedFromSurf(apf::castEntity(*ent),u);
      surf_elem++;
    }
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
