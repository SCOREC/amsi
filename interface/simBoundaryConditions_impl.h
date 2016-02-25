#include "apfBoundaryConditions.h"
#include "amsiNeumannIntegrators.h"
#include "simAnalysis.h"
#include "simAttributes.h"
#include "simWrapper.h"
#include <apfSIM.h>
namespace amsi
{
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
    buildBCs(pd,tp,bgn,nd,std::back_inserter(bcs));
    for(auto bc : bcs)
     *out++ = buildSimBCQuery(bc);
  }
  template <typename I>
    int applySimDirichletBCs(apf::Numbering * nm, pMesh msh, I bgn, I nd, double t)
  {
    int fxd = 0;
    for(auto it = bgn; it != nd; it++)
    {
      SimBCQuery * dir_bc = dynamic_cast<SimBCQuery*>(*it);
      SimBC * sim_bc = dir_bc->getSimBC();
      assert(sim_bc->tp == DIRICHLET);
      int dm = modelItemTypeDim(GEN_type((pGEntity)sim_bc->itm));
      std::list<pEntity> ents;
      getClassifiedDimEnts(msh,(pGEntity)sim_bc->itm,0,dm,std::back_inserter(ents));
      fxd += applyDirichletBC(nm,ents.begin(),ents.end(),dir_bc,t);
    }
    return fxd;
  }
  template <typename I>
    void applySimNeumannBCs(LAS * las, apf::Numbering * nm, pMesh msh, I bgn, I nd, double t)
  {
    apf::Field * fld = apf::getField(nm);
    for(auto it = bgn; it != nd; it++)
    {
      SimBCQuery * neu_bc = dynamic_cast<SimBCQuery*>(*it);
      SimBC * sim_bc = neu_bc->getSimBC();
      assert(sim_bc->tp == NEUMANN);
      NeumannIntegrator * i = buildNeumannIntegrator(las,fld,1,neu_bc,sim_bc->sbtp,t);
      std::list<pEntity> ents;
      int dm = modelItemTypeDim(GEN_type((pGEntity)sim_bc->itm));
      getClassifiedEnts(msh,(pGEntity)sim_bc->itm,dm,std::back_inserter(ents));
      applyNeumannBC(las,nm,ents.begin(),ents.end(),i,t);
    }
  }
  template <class O>
    void getDirichletBCAttributes(SimBC * bc, O out)
  {
    pAttribute att = GEN_attrib((pGEntity)bc->itm,getBCSubtypeString(bc->tp,bc->sbtp));
    switch(bc->sbtp)
    {
    case DISPLACEMENT:
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
    case FORCE:
      cutPaste<pAttribute>(Attribute_children(att),out); // either direction or magnitude (traction or follow force)
      break;
    default:
      *out++ = att;
    }
  }
  template <class O>
    void getBCAttributes(SimBC * bc, O out)
  {

    if(bc->tp == DIRICHLET)
      getDirichletBCAttributes(bc,out);
    else if(bc->tp == NEUMANN)
      getNeumannBCAttributes(bc,out);
  }
  template <class I, class O>
    void buildBCs(pACase ac, int tp, I begin, I end, O out)
  {
    for(I bc_tp = begin; bc_tp != end; ++bc_tp)
    {
      std::vector<pANode> bcs;
      getTypeNodes((pANode)ac,getBCSubtypeString(tp,*bc_tp),std::back_inserter(bcs));
      for(auto bc : bcs)
      {
        std::vector<pModelAssoc> mdl_ascs;
        cutPaste<pModelAssoc>(AttCase_findModelAssociations(ac,bc),
                              std::back_inserter(mdl_ascs));
        for(auto mdl_asc : mdl_ascs)
        {
          std::vector<pModelItem> mdl_itms;
          cutPaste<pModelItem>(AMA_modelItems(mdl_asc),std::back_inserter(mdl_itms));
          for(auto mdl_itm : mdl_itms)
          {
            SimBC * nw_bc = new SimBC;
            nw_bc->tp = tp;
            nw_bc->sbtp = *bc_tp;
            nw_bc->bc_nd = bc;
            nw_bc->itm = mdl_itm;
            *out++ = nw_bc;
          }
        }
      }
    }
  }
}
