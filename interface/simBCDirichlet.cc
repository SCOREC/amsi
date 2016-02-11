#include "simBCDirichlet.h"
namespace amsi
{
  DirichletSpecification buildSpecOnEntity(pGEntity entity,
                                           int dim,
                                           const std::string & attribute_name,
                                           const std::string & constraint_name,
                                           int component_offset)
  {
    DirichletSpecification result = {0,0,0,0.0};
    result.type = dim;
    result.tag = GEN_tag(entity);

    pAttribute dirichlet_constraint = GEN_attrib(entity, attribute_name.c_str());
    if(dirichlet_constraint)
    {
      pAttribute constraint_set = Attribute_childByType(dirichlet_constraint,"Set");
      pPList children = Attribute_children(constraint_set);
      void * iter = NULL;
      while(pAttribute att = static_cast<pAttribute>(PList_next(children,&iter)))
      {
        result.component = Attribute_infoType(att)[0] - component_offset; // assumes that the components of the field are designated by a single char, fine for 'XYZ' but not general
        pAttributeDouble constraint = static_cast<pAttributeDouble>(Attribute_childByType(att,constraint_name.c_str()));
        result.value = AttributeDouble_evalDT(constraint,0.0);
      }
    }
    return result;
  }

  void buildDirichletSpecFromSIM(pGModel model,
                                 const std::string & attribute_name,
                                 const std::string & constraint_name,
                                 int component_offset,
                                 std::vector<DirichletSpecification> & spec)
  {
    pGEntity entity = NULL;
    for(GRIter griter = GM_regionIter(model); (entity = GRIter_next(griter));)
      spec.push_back(buildSpecOnEntity(entity,
                                       3,
                                       attribute_name,
                                       constraint_name,
                                       component_offset));
    for(GFIter gfiter = GM_faceIter(model); (entity = GFIter_next(gfiter));)
      spec.push_back(buildSpecOnEntity(entity,
                                       2,
                                       attribute_name,
                                       constraint_name,
                                       component_offset));
    for(GEIter geiter = GM_edgeIter(model); (entity = GEIter_next(geiter));)
      spec.push_back(buildSpecOnEntity(entity,
                                       1,
                                       attribute_name,
                                       constraint_name,
                                       component_offset));
    for(GVIter gviter = GM_vertexIter(model); (entity = GVIter_next(gviter));)
      spec.push_back(buildSpecOnEntity(entity,
                                       0,
                                       attribute_name,
                                       constraint_name,
                                       component_offset));
  }
}
