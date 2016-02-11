#ifndef SIM_BOUNDARY_CONDITIONS_H_
#define SIM_BOUNDARY_CONDITIONS_H_
#include "simAnalysis.h"
#include "simAttributes.h"
#include <algorithm>
#include <cstring>
#include <iterator>
#include <vector>
namespace amsi
{
  char const * dir_bc_attrs[] =
  {
    "displacement"
  };
  char const * neu_bc_attrs[] =
  {
    "surface traction",
    "pressure"
  };
  /*
  class SimDirichletBC
  {
  private:
    const char * attr;
    std::vector<pModelItem> mdl_itms;
  public:
    SimDirichletBC(pACase pd, pANode nd, const char * at)
      : attr(at)
      , mdl_itms()
    {
      std::vector<pModelAssoc> mdl_ascs;
      cutPaste<pModelAssoc>(AttCase_findModelAssociations(pd,nd),
                            std::back_inserter(mdl_ascs));
      for(auto asc : mdl_ascs)
        cutPaste<pModelItem>(AMA_modelItems(asc),std::back_inserter(mdl_itms));
    }
    void apply(DirichletApplier * app)
    {
      for(itm : mdl_itms)
      {
        app->applyTo(itm,this);
        int nm_cmps = app->numComponents();
        for(int ii = 0; ii < nm_cmps; ii++)
        {
          if(isComponentFixed(itm,ii))
            app->fix(itm,ii,getComponentValue(itm,ii))
        }
      }
    }
    //
    virtual bool isComponentFixed(pModelItem itm, int ii);
    virtual void getComponentValue(pModelItem itm,
                                   int ii,
                                   double t = 0.0,
                                   double x = 0.0,
                                   double y = 0.0,
                                   double z = 0.0);
  };
  class SimDisplacementBC : public SimDirichletBC
  {
  private:
    static const char * cmps;
    pAttribute xyz[3];
  public:
    SimDisplacementBC(pACase pd, pANode nd)
      : SimDirichletBC(pd,nd,sim_dir_bc_attrs[0])
    { }
    virtual bool isComponentFixed(int ii)
    {
      return xyz[ii] != NULL;
    }
    virtual void getComponentValue(int ii,
                                   double t = 0.0,
                                   double x = 0.0,
                                   double y = 0.0,
                                   double z = 0.0)
    {
      AttributeTensor0_evalDT((pAttributeTensor0)xyz[ii],t);
    }
  };
  */
  struct SimDirichletBC
  {
    const char * dir_tp;
    pANode bc_nd;
    pModelItem itm; // the model entity
  };
  template <class O>
    void getDirichletAttributes(SimDirichletBC * bc, O out)
  {
    pAttribute att = GEN_attrib((pGEntity)bc->itm,bc->dir_tp);
    // displacement bc
    if(bc->dir_tp == dir_bc_attrs[0])
      cutPaste<pAttribute>(Attribute_children(att),out);
    else
      *out++ = att;
  }
  template <class O>
    void buildDirichletBCs(pACase ac, O out)
  {
    int nm_dir_bcs = sizeof(dir_bc_attrs) / sizeof(dir_bc_attrs[0]);
    for(int ii = 0; ii < nm_dir_bcs; ii++)
    {
      std::vector<pANode> bcs;
      amsi::getTypeNodes((pANode)ac,dir_bc_attrs[ii],std::back_inserter(bcs));
      for(auto bc : bcs)
      {
        std::vector<pModelAssoc> mdl_ascs;
        cutPaste<pModelAssoc>(AttCase_findModelAssociations(ac,bc),std::back_inserter(mdl_ascs));
        for(auto mdl_asc : mdl_ascs)
        {
          std::vector<pModelItem> mdl_itms;
          cutPaste<pModelItem>(AMA_modelItems(mdl_asc),std::back_inserter(mdl_itms));
          for(auto mdl_itm : mdl_itms)
          {
            SimDirichletBC * dir_bc = new SimDirichletBC;
            dir_bc->itm = mdl_itm;
            dir_bc->dir_tp = dir_bc_attrs[ii];
            dir_bc->bc_nd = bc;
            *out++ = dir_bc;
          }
        }
      }
    }
  }
  void applyDirichletBC(SimDirichletBC * bc, pMesh msh)
  {
    std::vector<pAttribute> atts;
    getDirichletAttributes(bc,std::back_inserter(atts));
    std::list<pEntity> ents;
    int dm = modelItemTypeDim(GEN_type((pGEntity)bc->itm));
    for(int d = dm; d >= 0; d--)
      getClassifiedEnts(msh,(pGEntity)bc->itm,d,std::back_inserter(ents));
  }
  void processProblemDefinition(pACase ac)
  {
    //buildDirichletBCs(ac);
    // build/apply neumann bcs
  }
}
#endif
