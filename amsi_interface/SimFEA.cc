#include "SimFEA.h"

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
	while (pr = RIter_next(it))classified.push_back((pEntity)pr);
	RIter_delete(it);
	break;
      }
      case 2:
      {
	FIter it = M_classifiedFaceIter(mesh,entity,1);
	pFace pf;
	while (pf = FIter_next(it))classified.push_back((pEntity)pf);
	FIter_delete(it);
	break;
      }
      case 1:
      {
	EIter it = M_classifiedEdgeIter(mesh,entity,1);
	pEdge pe;
	while (pe = EIter_next(it))classified.push_back((pEntity)pe);
	EIter_delete(it);
	break;
      }
      case 0:
      {
	VIter it = M_classifiedVertexIter(mesh,entity,1);
	pVertex pv;
	while (pv = VIter_next(it))classified.push_back((pEntity)pv);
	VIter_delete(it);
	break;
      }
      }
    }

    void InitModelAttributes(pGModel model,std::vector<std::string> & attribute_names)
    {
      pAManager attribute_manager = SModel_attManager(model);
      pProgress progress = Progress_new();
      for(auto attribute_name = attribute_names.begin(); attribute_name != attribute_names.end(); attribute_name++)
      {
	pACase constraints = AMAN_findCase(attribute_manager,attribute_name->c_str());
	if(constraints)
	{
	  pPList children = AttNode_children(constraints);
	  pACase child;
	  void * iter = NULL;
	  while(child = (pACase)PList_next(children,&iter))
	    AttCase_setModel(child,model);
	  AttCase_setModel(constraints,model);	
	  AttCase_associate(constraints,progress);
	}
      }
      Progress_delete(progress);
    }
    
    SimFEA::SimFEA(MPI_Comm comm,
		   const std::string & in_analysis_name,
		   const std::string & in_model_name,
		   pGModel in_model,
		   const std::string & in_mesh_name,
		   pParMesh in_mesh) :
      FEA(comm,in_analysis_name),
      model_name(in_model_name),
      model(in_model),
      mesh_name(in_mesh_name),
      mesh(in_mesh)
    {
      part = PM_mesh(mesh,0);
    }

    SimFEA::SimFEA(MPI_Comm comm,
		   const std::string & in_analysis_name,
		   pGModel in_model,
		   pParMesh in_mesh) :
      FEA(comm,in_analysis_name),
      model_name("[unavailable]"),
      model(in_model),
      mesh_name("[unavailable]"),
      mesh(in_mesh)
    {
      part = PM_mesh(in_mesh,0);
    }

    SimFEA::SimFEA(const std::string & in_analysis_name,
		   const std::string & in_model_name,
		   pGModel in_model,
		   const std::string & in_mesh_name,
		   pParMesh in_mesh) :
      FEA(MPI_COMM_WORLD, in_analysis_name),
      model_name(in_model_name),
      model(in_model),
      mesh_name(in_mesh_name),
      mesh(in_mesh)
    {
      part = PM_mesh(mesh,0);
    }

    SimFEA::SimFEA(const std::string & in_analysis_name,
		   pGModel in_model,
		   pParMesh in_mesh) :
      FEA(MPI_COMM_WORLD, in_analysis_name),
      model_name("[unavailable]"),
      model(in_model),
      mesh_name("[unavailable]"),
      mesh(in_mesh)
    {
      part = PM_mesh(in_mesh,0);
    }

    void SimFEA::Adapt()
    {
      pMSAdapt adapter = MSA_new(mesh,1);

      // .. make calls to set mesh sizes at nodes

      MSA_adapt(adapter,NULL);
      MSA_delete(adapter);
    }

  }
}
