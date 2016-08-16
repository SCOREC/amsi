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
  double measureVol_pGFace(pGFace pGF, int tag, pMesh msh, apf::Field * u)
  {
    /**
     * pGFace ent:     face entity of geometric model.
     * pMesh msh:      partitioned mesh.
     * apf::Field * u: displacement field. */
     double vol = 0.0;
    /** Extract face entities of mesh that lie on face entities of geometric model */
     std::list<pEntity> ents;
     getClassifiedEnts(msh,pGF,2,std::back_inserter(ents));
    /** for debugging purposes */
     AMSI_DEBUG (
      std::vector<int> tags;
      int ent_size = 0;
      for (int ii=ent_size; ii<ents.size(); ii++)
      {
        tags.push_back(GEN_tag(pGF));
        ent_size = ents.size();
      }
      )
     int surf_elem = 0;
     apf::Mesh * apfmsh = apf::getMesh(u);
     for(std::list<pEntity>::iterator ent = ents.begin(); ent != ents.end(); ++ent)
     {
      if(apfmsh->isOwned(apf::castEntity(*ent)))
      {
        pRegion mshRgn0 = F_region((pFace)(*ent),0);
        pRegion mshRgn1 = F_region((pFace)(*ent),1);
        int GRgn0_tag, GRgn1_tag, normal_dir;
        if (mshRgn0 != NULL)
          GRgn0_tag = GEN_tag(R_whatIn(mshRgn0));
        else
          GRgn0_tag = -2;
        if (mshRgn1 != NULL)
          GRgn1_tag = GEN_tag(R_whatIn(mshRgn1));
        else
          GRgn1_tag = -2;
        if (GRgn0_tag == tag)
          normal_dir = 1;
        else
          normal_dir = -1;

        AMSI_DEBUG (
          std::cout<<"surf_elem:"<<surf_elem<<" belongs to face: "<<tags[surf_elem]
          <<" Rgn "<<GRgn0_tag<<" inside,"
          <<" Rgn "<<GRgn1_tag<<" outside,"
          <<" normal direction "<<normal_dir<<std::endl;
          )
        vol += measureDisplacedFromSurf(apf::castEntity(*ent),u,normal_dir);
        surf_elem++;
      }
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
