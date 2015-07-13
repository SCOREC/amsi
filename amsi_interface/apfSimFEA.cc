#include "apfSimFEA.h"
#include "SimTensorFieldQuery.h"
#include <amsiMPI.h>

#include <apfSIM.h>
#include <apfShape.h>

namespace amsi {
  namespace Analysis {

    apfSimFEA::apfSimFEA(MPI_Comm comm,
			 const std::string & in_analysis_name,
			 pGModel in_model,
			 pParMesh in_mesh)
      : FEA(comm,in_analysis_name),
	apfFEA(comm,apf::createMesh(in_mesh)),
	SimFEA(in_analysis_name,
	       in_model,
	       in_mesh),
	mesh_adapted(false)
    { }

    apfSimFEA::apfSimFEA(const std::string & in_analysis_name,
			 const std::string & in_model_name,
			 pGModel in_model,
			 const std::string & in_mesh_name,
			 pParMesh in_mesh)
      : FEA(AMSI_COMM_LOCAL,in_analysis_name),
        apfFEA(AMSI_COMM_LOCAL,apf::createMesh(in_mesh)),
	SimFEA(in_analysis_name,
	       in_model_name,
	       in_model,
	       in_mesh_name,
	       in_mesh),
	mesh_adapted(false)
    { }
    
    apfSimFEA::apfSimFEA(const std::string & in_analysis_name,
			 pGModel in_model,
			 pParMesh in_mesh)
      : FEA(AMSI_COMM_LOCAL,in_analysis_name),
        apfFEA(AMSI_COMM_LOCAL,apf::createMesh(in_mesh)),
	SimFEA(in_analysis_name,
	       in_model,
	       in_mesh),
	mesh_adapted(false)
    { }
    
    void apfSimFEA::ApplyBC_Dirichlet()
    {
      fixed_dofs = 0;
      pGEntity entity;
      if(analysis_dim == 3)
      {
	for(GRIter griter = GM_regionIter(model); entity = GRIter_next(griter);)
	  fixed_dofs += Entity_ApplyBC_Dirichlet(entity,3);
      }
      if(analysis_dim >= 2)
      {
	for(GFIter gfiter = GM_faceIter(model); entity = GFIter_next(gfiter);)
	  fixed_dofs += Entity_ApplyBC_Dirichlet(entity,2);
      }
      for(GEIter geiter = GM_edgeIter(model); entity = GEIter_next(geiter);)
	fixed_dofs += Entity_ApplyBC_Dirichlet(entity,1);

      for(GVIter gviter = GM_vertexIter(model); entity = GVIter_next(gviter);)
	fixed_dofs += Entity_ApplyBC_Dirichlet(entity,0);
      
      std::cout << "There are " << fixed_dofs << " dofs fixed by essential boundary conditions." << std::endl;
    }

    int apfSimFEA::Entity_ApplyBC_Dirichlet(pGEntity entity, int dim)
    {
      int fixed_dofs = 0;

      pAttribute disp_constraint = GEN_attrib(entity,"displacement constraint");
      if(disp_constraint)
      {
	pAttribute constraint_set = Attribute_childByType(disp_constraint,"Set");
	double disp[analysis_dim];
	bool fixed[analysis_dim];
	for(int ii = 0; ii < analysis_dim; ii++)
	{
	  disp[ii] = 0.0;
	  fixed[ii] = false;
	}

	pPList children = Attribute_children(constraint_set);
	void * iter = NULL;
	while(pAttribute att = static_cast<pAttribute>(PList_next(children,&iter)))
	{
	  int axis = Attribute_infoType(att)[0] - 'X'; // assuming 'xyz' axes... not too big an assumption
	  std::cout << Attribute_infoType(att) << std::endl;
	  pAttributeDouble disp_attribute = static_cast<pAttributeDouble>(Attribute_childByType(att,"Total Displacement"));
	  disp[axis] = AttributeDouble_evalDT(disp_attribute,simulation_time*1.0);
	  fixed[axis] = true;
	}
	PList_delete(children);

	// get all the entities of the same or lower dimensionality classified on the geometric entity
	int num_components = apf::countComponents(apf_primary_field);
	for(int current_dim = dim; current_dim >= 0; current_dim--)
	{
	  std::list<pEntity> classified;
	  Model_GetClassifiedEntities(part,entity,current_dim,classified);

	  for(std::list<pEntity>::iterator iter = classified.begin(), iterend = classified.end(); iter != iterend; iter++)
	  {
	    apf::MeshEntity * mesh_ent = apf::castEntity(*iter);
	    apf::FieldShape * fs = apf::getShape(apf_primary_field);

	    int num_nodes = fs->countNodesOn(apf_mesh->getType(mesh_ent));
	    bool needs_set = false;

	    for(int ii = 0; ii < num_nodes; ii++)
	    {

              apf::Vector3 cdisp;
              apf::getVector(apf_primary_field,mesh_ent,ii,cdisp);

	      for(int jj = 0; jj < num_components; jj++)
	      {
		needs_set = needs_set ? true : fixed[jj];
		if(fixed[jj])
		{
		  apf::fix(apf_primary_numbering,
			   mesh_ent,
			   ii, // node
			   jj, // component
			   true);
                  cdisp[jj] = disp[jj];
		  fixed_dofs++;
		}
	      }

	      if(needs_set)
		apf::setVector(apf_primary_field,mesh_ent,ii,cdisp);
	    }
	  }
	}
      }
      return fixed_dofs;
    }

    void apfSimFEA::ApplyBC_Neumann(LAS * las)
    {
      assert(neumann_integrator);
      
      // iterate over the model
      pGEntity entity;
      for(GFIter gfiter = GM_faceIter(model); entity = GFIter_next(gfiter);)
	Entity_ApplyBC_Neumann(las,entity,2);
      
      for(GEIter geiter = GM_edgeIter(model); entity = GEIter_next(geiter);)
	Entity_ApplyBC_Neumann(las,entity,1);
      
      for(GVIter gviter = GM_vertexIter(model); entity = GVIter_next(gviter);)
	Entity_ApplyBC_Neumann(las,entity,0);
    }
    
    void apfSimFEA::Entity_ApplyBC_Neumann(LAS * las,
					   pGEntity entity, 
					   int dim)
    {
      pAttribute force_constraint = GEN_attrib(entity,"force constraint");
      if(force_constraint)
      {
	int num_components = apf::countComponents(apf_primary_field);
	pAttributeTensor1 force_attrib = static_cast<pAttributeTensor1>(Attribute_childByType(force_constraint,"direction"));

	// todo: abstract TensorFieldQuery*?
	TensorFieldQueryT<pAttributeTensor1> force_value(force_attrib);
	
	double simulation_time = 0.0;
	force_value.bind(4,simulation_time);
	neumann_integrator->setTensorQuery(&force_value);
	std::list<pEntity> classified;
	Model_GetClassifiedEntities(part,entity,dim,classified);
	
	apf::MeshEntity * current_element = NULL;
	for(std::list<pEntity>::iterator iter = classified.begin(), iterend = classified.end(); iter != iterend; iter++)
	{
	  current_element = apf::castEntity(*iter);
	  apf::MeshElement * me = apf::createMeshElement(apf_mesh,current_element);
	  neumann_integrator->process(me);
	  apf::NewArray<int> ids;
	  apf::getElementNumbers(apf_primary_numbering,current_element,ids);
	  apf::NewArray<apf::Vector3> node_values;
	  AssembleDOFs(las,
		       neumann_integrator->numDofs(),
		       &ids[0],
		       &node_values[0], // node_values
		       NULL, // Ke
		       &neumann_integrator->getFe()(0),
		       false);
	}
      }
    }
    
    void apfSimFEA::Assemble(LAS * las)
    {
      ApplyBC_Neumann(las);
      apfFEA::Assemble(las);
    }
    
  }
}
