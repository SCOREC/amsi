#ifndef SIM_BOUNDARY_CONDITIONS_IMPL_H_
#define SIM_BOUNDARY_CONDITIONS_IMPL_H_
#include "apfBoundaryConditions.h"
#include "simClassified.h"
#include "amsiNeumannIntegrators.h"
#include "amsiFields.h"
#include "simAnalysis.h"
#include "simAttributes.h"
#include "simWrapper.h"
#include <apfSIM.h>
#include <unordered_map>
namespace amsi
{
  typedef std::unordered_map<apf::MeshEntity*,std::vector<bool>> bc_set_map;
  template <typename I>
    SimBCQuery * findSimBCQueryOn(I bgn, I nd, pGEntity ent)
  {
    for(auto it = bgn; it != nd; it++)
    {
      SimBCQuery * qry = reinterpret_cast<SimBCQuery*>(*it);
      if(ent == (pGEntity)qry->getSimBC()->itm)
        return qry;
    }
    return NULL;
  }
  template <typename I, typename O>
    void buildSimBCQueries(pACase pd, int tp, I bgn, I nd, O out)
  {
    std::vector<SimBC*> bcs;
    buildSimBCs(pd,tp,bgn,nd,std::back_inserter(bcs));
    for(auto bc = bcs.begin(); bc != bcs.end(); ++bc)
      *out++ = buildSimBCQuery(*bc);
  }
  template <typename O>
    void buildBCQueriesFromSim(pACase pd, int tp, O out)
  {
    std::vector<SimBC*> bcs;
    buildSimBCs(pd,tp,std::back_inserter(bcs)); // prob_def, dirichlet, SimBC output
    for(auto bc = bcs.begin(); bc != bcs.end(); ++bc)
      *out++ = buildSimBCQuery(*bc);
  }
  template <typename I>
    void destroySimBCQueries(pACase pd, int tp, I bgn, I nd)
    {
    }
  /*
  template <typename I1, typename I2>
    int applyAllSimDirichletBCs(I1 nm_bgn, I2 nm_nd, pMesh msh, I2 bgn, I2 nd, double t)
  {
    int fxd = 0;
    for(auto nm = nm_bgn; nm != nm_nd; ++nm)
    {
      // gather appropriate bcqueries
      std::vector<SimBCQuery*> bcs;
      //std::copy_if(bgn,nd,std::back_inserter(bcs),[&](I2 bc)->bool{;});
      //applySimDirichletBCs(*nm,msh,,,t);
    }
    return fxd;
  }
  */
  template <typename I>
    int applySimDirichletBCs(apf::Numbering * nm, pMesh, I bgn, I nd, double t, apf::Field * delta_field)
  {
    bc_set_map already_set_map;
    int fxd = 0;
    for(auto it = bgn; it != nd; it++)
    {
      SimBCQuery * dir_bc = dynamic_cast<SimBCQuery*>(*it);
      SimBC * sim_bc = dir_bc->getSimBC();
      assert(sim_bc->tp == BCType::dirichlet);
      int dm = modelItemTypeDim(GEN_type((pGEntity)sim_bc->itm));
      for(int ii = 0; ii <= dm; ++ii)
      {
        auto bgn = amsi::beginClassified(apf::getMesh(apf::getField(nm)),reinterpret_cast<apf::ModelEntity*>(sim_bc->itm),ii);
        auto end = amsi::endClassified(bgn);
        fxd += applyDirichletBC(nm,bgn,end,*it,t,already_set_map,delta_field);
      }
    }
    return fxd;
  }
  template <typename I>
    void applySimNeumannBCs(LAS * las, apf::Numbering * nm, pMesh, I bgn, I nd, double t)
  {
    apf::Field * fld = apf::getField(nm);
    for(auto it = bgn; it != nd; it++)
    {
      SimBCQuery * neu_bc = dynamic_cast<SimBCQuery*>(*it);
      SimBC * sim_bc = neu_bc->getSimBC();
      assert(sim_bc->tp == BCType::neumann);
      NeumannIntegrator * i = buildNeumannIntegrator(las,fld,1,neu_bc,sim_bc->sbtp,t);
      int dm = modelItemTypeDim(GEN_type((pGEntity)sim_bc->itm));
      auto bgn = amsi::beginClassified(apf::getMesh(apf::getField(nm)),reinterpret_cast<apf::ModelEntity*>(sim_bc->itm),dm);
      auto end = amsi::endClassified(bgn);
      applyNeumannBC(las,nm,bgn,end,i,t);
      deleteNeumannIntegrator(i);
    }
  }
  template <class O>
    void getDirichletBCAttributes(SimBC * bc, O out)
  {
    pAttribute att = GEN_attrib((pGEntity)bc->itm,getBCSubtypeString(bc->tp,bc->sbtp));
    switch(bc->sbtp)
    {
    case FieldUnit::unitless:
    case FieldUnit::displacement:
      cutPaste<pAttribute>(Attribute_children(att),out);
      break;
    default:
      *out++ = att;
    }
  }
  template <class O>
    void getNeumannBCAttributes(SimBC * bc, O out)
  {
    pAttribute att = GEN_attrib((pGEntity)bc->itm,getBCSubtypeString(bc->tp,bc->sbtp));
    switch(bc->sbtp)
    {
    case NeuBCType::custom:
    case NeuBCType::traction:
    case NeuBCType::pressure:
      cutPaste<pAttribute>(Attribute_children(att),out); // either direction or magnitude (traction or follow force)
      break;
    default:
      *out++ = att;
    }
  }
  template <class O>
    void getBCAttributes(SimBC * bc, O out)
  {

    if(bc->tp == BCType::dirichlet)
      getDirichletBCAttributes(bc,out);
    else if(bc->tp == BCType::neumann)
      getNeumannBCAttributes(bc,out);
  }
  template <class O>
    void getAssociatedModelItems(pACase cs, pANode nd, O out)
  {
    std::vector<pModelAssoc> mdl_ascs;
    cutPaste<pModelAssoc>(AttCase_findModelAssociations(cs,nd),std::back_inserter(mdl_ascs));
    auto mdl_asc_nd = mdl_ascs.end();
    for(auto mdl_asc = mdl_ascs.begin(); mdl_asc != mdl_asc_nd; ++mdl_asc)
    {
      std::vector<apf::ModelEntity*> mdl_itms;
      cutPaste<apf::ModelEntity*>(AMA_modelItems(*mdl_asc),std::back_inserter(mdl_itms));
      std::copy(mdl_itms.begin(),mdl_itms.end(),out);
    }
  }
  template <class I, class O>
    void buildSimBCs(pACase ac, int tp, I bgn, I nd, O out)
  {
    for(I bc_tp = bgn; bc_tp != nd; ++bc_tp)
    {
      std::vector<pANode> bcs;
      getTypeNodes((pANode)ac,getBCSubtypeString(tp,*bc_tp),std::back_inserter(bcs));
      auto bc_nd = bcs.end();
      for(auto bc = bcs.begin(); bc != bc_nd; ++bc)
      {
        std::vector<apf::ModelEntity * > mdl_itms;
        getAssociatedModelItems(ac,*bc,std::back_inserter(mdl_itms));
        auto mdl_itm_nd = mdl_itms.end();
        for(auto mdl_itm = mdl_itms.begin(); mdl_itm != mdl_itm_nd; ++mdl_itm)
        {
          pGEntity mdl_ent = reinterpret_cast<pGEntity>(*mdl_itm);
          assert(ModelItem_isGEntity(mdl_ent));
          SimBC * nw_bc = new SimBC;
          nw_bc->tp = tp;
          nw_bc->sbtp = *bc_tp;
          nw_bc->bc_nd = *bc;
          nw_bc->itm = mdl_ent;
          *out++ = nw_bc;
        }
      }
    }
  }
  template <typename O>
    void buildSimBCs(pACase cs, int tp, O out)
  {
    std::vector<pANode> bcs;
    getTypeNodes((pANode)cs,getBCTypeString(tp),std::back_inserter(bcs));
    for(auto bc = bcs.begin(); bc != bcs.end(); ++bc)
    {
      pANode fld_nm = AttNode_childByType(*bc,"field name");
      std::string nm = std::string(AttInfoString_value((pAttInfoString)fld_nm));
      std::vector<apf::ModelEntity*> mdl_itms;
      getAssociatedModelItems(cs,*bc,std::back_inserter(mdl_itms));
      for(auto mdl_itm = mdl_itms.begin(); mdl_itm != mdl_itms.end(); ++mdl_itm)
      {
        pGEntity mdl_ent = reinterpret_cast<pGEntity>(*mdl_itm);
        SimBC * nw_bc = new SimBC;
        nw_bc->tp = tp;
        nw_bc->sbtp = 0;
        nw_bc->bc_nd = *bc;
        nw_bc->itm = mdl_ent;
        nw_bc->fld = nm;
        *out++ = nw_bc;
      }
    }
  }
}
#endif
