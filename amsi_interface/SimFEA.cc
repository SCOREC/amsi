#include "SimFEA.h"
#include <cassert>

namespace amsi {
  namespace Analysis {
    
    template <>
    void Model_PrintInfo<pGModel>(pGModel model, std::ostream & out)
    {

    }

    template <>
    void Mesh_PrintInfo<pMesh>(pMesh mesh, std::ostream & out)
    {
      out << "The specified mesh or mesh-part has: " << std::endl;
      out << "\t" << M_numRegions(mesh) << " regions." << std::endl;
      out << "\t" << M_numFaces(mesh) << " faces." << std::endl;
      out << "\t" << M_numEdges(mesh) << " edges." << std::endl;
      out << "\t" << M_numVertices(mesh) << " vertices." << std::endl;
    }

   void Model_GetClassifiedEntities(pMesh mesh,
				    pGEntity entity,
				    int dim,
				    std::list<pEntity> & classified)
    {
      switch(dim)
      {
      case 3:
      {
	RIter it = M_classifiedRegionIter(mesh,entity);
	pRegion pr;
	while ((pr = RIter_next(it))) classified.push_back((pEntity)pr);
	RIter_delete(it);
	break;
      }
      case 2:
      {
	FIter it = M_classifiedFaceIter(mesh,entity,1);
	pFace pf;
	while ((pf = FIter_next(it))) classified.push_back((pEntity)pf);
	FIter_delete(it);
	break;
      }
      case 1:
      {
	EIter it = M_classifiedEdgeIter(mesh,entity,1);
	pEdge pe;
	while ((pe = EIter_next(it))) classified.push_back((pEntity)pe);
	EIter_delete(it);
	break;
      }
      case 0:
      {
	VIter it = M_classifiedVertexIter(mesh,entity,1);
	pVertex pv;
	while ((pv = VIter_next(it))) classified.push_back((pEntity)pv);
	VIter_delete(it);
	break;
      }
      }
    }

    void initAttributeCase(pGModel model,const std::string & attr_nm)
    {
      pAManager attribute_manager = SModel_attManager(model);
      pProgress progress = Progress_new();
      pACase constraints = AMAN_findCase(attribute_manager,attr_nm.c_str());
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

    void clearModelAttributes(pGModel mdl) {}

    bool isIncrementallyLoaded(pGEntity ent)
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
    
    bool requiresIncrementalLoading(pGModel mdl)
    {
      bool result = false;
      pGEntity entity;
      for(GFIter gfiter = GM_faceIter(mdl); (entity = GFIter_next(gfiter)) && !result;)
	result = isIncrementallyLoaded(entity);
      for(GEIter geiter = GM_edgeIter(mdl); (entity = GEIter_next(geiter)) && !result;)
	result = isIncrementallyLoaded(entity);
      for(GVIter gviter = GM_vertexIter(mdl); (entity = GVIter_next(gviter)) && !result;)
	result = isIncrementallyLoaded(entity);
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
    
    SimFEA::SimFEA(MPI_Comm comm,
		   const std::string & in_analysis_name,
		   const std::string & in_model_name,
		   pGModel in_model,
		   const std::string & in_mesh_name,
		   pParMesh in_mesh) :
      FEA(comm,in_analysis_name),
      model_name(in_model_name),
      mesh_name(in_mesh_name),
      model(in_model),
      mesh(in_mesh),
      part(),
      should_adapt(false),
      sim_size_field(),
      fields_to_map()
    {
      part = PM_mesh(mesh,0);
      fields_to_map = PList_new();
    }

    SimFEA::SimFEA(MPI_Comm comm,
		   const std::string & in_analysis_name,
		   pGModel in_model,
		   pParMesh in_mesh) :
      FEA(comm,in_analysis_name),
      model_name("[unavailable]"),
      mesh_name("[unavailable]"),
      model(in_model),
      mesh(in_mesh),
      part(),
      should_adapt(false),
      sim_size_field(),
      fields_to_map()
    {
      part = PM_mesh(in_mesh,0);
      fields_to_map = PList_new();
    }

    SimFEA::SimFEA(const std::string & in_analysis_name,
		   const std::string & in_model_name,
		   pGModel in_model,
		   const std::string & in_mesh_name,
		   pParMesh in_mesh) :
      FEA(MPI_COMM_WORLD, in_analysis_name),
      model_name(in_model_name),
      mesh_name(in_mesh_name),
      model(in_model),
      mesh(in_mesh),
      part(),
      should_adapt(false),
      sim_size_field(),
      fields_to_map()
    {
      part = PM_mesh(mesh,0);
      fields_to_map = PList_new();
    }

    SimFEA::SimFEA(const std::string & in_analysis_name,
		   pGModel in_model,
		   pParMesh in_mesh) :
      FEA(MPI_COMM_WORLD, in_analysis_name),
      model_name("[unavailable]"),
      mesh_name("[unavailable]"),
      model(in_model),
      mesh(in_mesh),
      part(),
      should_adapt(false),
      sim_size_field(),
      fields_to_map()
    {
      part = PM_mesh(in_mesh,0);
      fields_to_map = PList_new();
    }

    void SimFEA::Adapt()
    {
      assert(sim_size_field);
      if(should_adapt)
      {
	pMSAdapt adapter = MSA_new(mesh,1);
	MSA_setMapFields(adapter,fields_to_map);
	
	// set mesh size from size field
	pVertex vert = NULL;
	for(VIter viter = M_vertexIter(part); (vert = VIter_next(viter)); )
	{
	  pDofGroup dof = Field_entDof(sim_size_field,(pEntity)vert,0);
	  double size = DofGroup_value(dof,0,0);
	  MSA_setVertexSize(adapter,vert,size);
	}
	
	MSA_adapt(adapter,NULL);
	MSA_delete(adapter);
      }
    }

    void SimFEA::addFieldToMap(pField fd)
    {
      if(!PList_contains(fields_to_map,static_cast<void*>(fd)))
	fields_to_map = PList_append(fields_to_map,fd);
    }
  }
}
