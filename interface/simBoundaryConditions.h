#ifndef SIM_BOUNDARY_CONDITIONS_H_
#define SIM_BOUNDARY_CONDITIONS_H_
#include "simAnalysis.h"
#include "simAttributes.h"
#include <algorithm>
#include <cassert>
#include <cstring>
#include <iterator>
#include <vector>
namespace amsi
{
  enum BCTypes
  {
    DIRICHLET,
    NEUMANN,
    NUM_BC_TYPES
  };
  enum DirichletBCType
  {
    DISPLACEMENT,
    NUM_DIRICHLET_TYPES
  };
  enum NeumannBCType
  {
    SURFACE_TRACTION,
    PRESSURE,
    NUM_NEUMANN_TYPES
  };
  char const * getBCTypeString(int tp);
  char const * getBCSubtypeString(int tp, int sbpt);
  /**
   * Get a c-style string describing the dirichlet boundary condition type.
   */
  char const * getDirichletTypeString(int tp);
  /**
   * Get a c-style string describing the neumann boundary condition type.
   */
  char const * getNeumannTypeString(int tp);
  struct SimBC
  {
    int tp;
    int sbtp;
    pANode bc_nd;
    pModelItem itm; // the model entity
  };
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
    *out++ = att;
  }
  template <class O>
    void getBCAttributes(SimBC * bc, O out)
  {

    if(bc->tp == DIRICHLET)
      getDirichletBCAttributes(bc,out);
    else if(bc->tp == NEUMANN)
      getNeumannBCAttributes(bc,out);
  }
  template <class O>
    void buildDirichletBCs(pACase ac, O out)
  {
    for(int ii = 0; ii < NUM_DIRICHLET_TYPES; ii++)
    {
      std::vector<pANode> bcs;
      amsi::getTypeNodes((pANode)ac,getDirichletTypeString(ii),std::back_inserter(bcs));
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
            SimBC * dir_bc = new SimBC;
            dir_bc->tp = DIRICHLET;
            dir_bc->sbtp = ii;
            dir_bc->bc_nd = bc;
            dir_bc->itm = mdl_itm;
            *out++ = dir_bc;
          }
        }
      }
    }
  }
  void applyBC(SimBC * bc, pMesh msh);
//  void applyDirichletBC(SimBC * bc, pMesh msh);
//  void processProblemDefinition(pACase ac);
}
#endif
