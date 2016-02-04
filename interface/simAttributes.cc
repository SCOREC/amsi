#include "simAttributes.h"
namespace amsi
{
  void initAttributeCase(pGModel model,const char * att_cs)
  {
    pAManager attribute_manager = SModel_attManager(model);
    pProgress progress = Progress_new();
    pACase constraints = AMAN_findCase(attribute_manager,att_cs);
    if(constraints)
    {
      pPList children = AttNode_children(constraints);
      pACase child;
      void * iter = NULL;
      while((child = (pACase)PList_next(children,&iter)))
        AttCase_setModel(child,model);
      AttCase_setModel(constraints,model);
      AttCase_associate(constraints,progress);
    }
    Progress_delete(progress);
  }
  void clearModelAttributes(pGModel mdl)
  {
    // get the associated attribute case, call AttCase_unassociate()
  }
  bool isIncrementallyLoaded(pGEntity ent, const char * attr)
  {
    bool result = false;
    pAttribute force_constraint = GEN_attrib(ent,"force constraint");
    if(force_constraint)
      result = !AttributeTensor1_constant(static_cast<pAttributeTensor1>(Attribute_childByType(force_constraint,"direction")));
    pAttribute disp_constraint = GEN_attrib(ent,"displacement constraint");
    if(disp_constraint)
    {
      pAttribute constraint_set = Attribute_childByType(disp_constraint,"Set");
      pPList children = Attribute_children(constraint_set);
      void * iter = NULL;
      pAttribute att;
      while((att = static_cast<pAttribute>(PList_next(children,&iter))) && !result)
      {
        pAttributeDouble disp_attribute =
          static_cast<pAttributeDouble>(Attribute_childByType(att,"Total Displacement"));
        result = !AttributeDouble_constant(disp_attribute);
      }
      PList_delete(children);
    }
    return result;
  }
  bool requiresIncrementalLoading(pGModel mdl, const char * attr)
  {
    bool result = false;
    pGEntity entity;
    for(GFIter gfiter = GM_faceIter(mdl); (entity = GFIter_next(gfiter)) && !result;)
      result = isIncrementallyLoaded(entity,attr);
    for(GEIter geiter = GM_edgeIter(mdl); (entity = GEIter_next(geiter)) && !result;)
      result = isIncrementallyLoaded(entity,attr);
    for(GVIter gviter = GM_vertexIter(mdl); (entity = GVIter_next(gviter)) && !result;)
      result = isIncrementallyLoaded(entity,attr);
    return result;
  }
  bool hasAttribute(pGEntity entity, const char * attr)
  {
    return GEN_attrib(entity,attr) != NULL;
  }
  void getWithAttribute(pGModel mdl, const char * attr, std::list<pGEntity> & ents)
  {
    pGEntity entity;
    for(GRIter griter = GM_regionIter(mdl); (entity = GRIter_next(griter)); )
      if(hasAttribute(entity,attr))
        ents.push_back(entity);
    for(GFIter gfiter = GM_faceIter(mdl); (entity = GFIter_next(gfiter)); )
      if(hasAttribute(entity,attr))
        ents.push_back(entity);
    for(GEIter geiter = GM_edgeIter(mdl); (entity = GEIter_next(geiter)); )
      if(hasAttribute(entity,attr))
        ents.push_back(entity);
    for(GVIter gviter = GM_vertexIter(mdl); (entity = GVIter_next(gviter)); )
      if(hasAttribute(entity,attr))
        ents.push_back(entity);
  }
  void applyRegionAttributes(pGModel mdl, Analysis::FEA * fea)
  {
    pGEntity rgn = NULL;
    for(GRIter griter = GM_regionIter(mdl); (rgn = GRIter_next(griter)); )
    {
      int tg = GEN_tag(rgn);
      std::cout << "Processing region " << tg << ":" << std::endl;
      pPList atts = GEN_attributes(rgn,"");
      pAttribute att = NULL;
      void * iter = NULL;
      while( (att = (pAttribute)PList_next(atts,&iter)) )
      {
        char * nm = Attribute_name(att);
        char * inf_tp = Attribute_infoType(att);
        std::cout << "\tAttribute " << nm << " of type " << inf_tp << std::endl;
      }
      PList_delete(atts);
    }
  }
  void applyAttributes(Analysis::SimFEA* fea)
  {
    pGModel mdl = fea->getGeometricDomain();
    applyRegionAttributes(mdl,fea);
  }
}
