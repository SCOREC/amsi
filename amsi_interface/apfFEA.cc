#include "apfFEA.h"

#include <apfShape.h>

namespace amsi {
  namespace Analysis {

    void apfFEA::RenumberDOFs()
    {
      if(!numbered)
      {
	local_dof_count = apf::AdjReorder(apf_primary_numbering);
	
	int analysis_size, analysis_rank;
	MPI_Comm_rank(analysis_comm,&analysis_rank);
	MPI_Comm_size(analysis_comm,&analysis_size);
	
	int dofs[analysis_size];
	memset(&dofs[0],0,analysis_size*sizeof(int));

	dofs[analysis_rank] = local_dof_count;
	MPI_Allgather(&dofs[analysis_rank],1,MPI_INTEGER,
		      &dofs[0],1,MPI_INTEGER,
		      analysis_comm);
	
	for(int ii = 0; ii < analysis_size; ii++)
	{
	  if(ii == analysis_rank)
	    first_local_dof = global_dof_count;
	  global_dof_count += dofs[ii];
	}
	
	if(first_local_dof > 0)
	  apf::SetNumberingOffset(apf_primary_numbering,first_local_dof);
	
	apf::synchronize(apf_primary_numbering);
	numbered = true;
      }
    }

    void apfFEA::Assemble(LAS * las)
    {
      assert(elemental_system);
      apf::MeshEntity * me = NULL;
      for(apf::MeshIterator * it = apf_mesh->begin(analysis_dim);
	  me = apf_mesh->iterate(it);)
      {
	apf::MeshElement * melm = apf::createMeshElement(apf_mesh,me);
	elemental_system->process(melm);
	apf::NewArray<apf::Vector3> dofs;
	apf::getVectorNodes(elemental_system->getElement(),dofs);
	apf::NewArray<int> ids;
	apf::getElementNumbers(apf_primary_numbering,me,ids);
	AssembleDOFs(las,
		     elemental_system->numElementalDOFs(),
		     &ids[0],
		     &dofs[0],
		     &elemental_system->getKe()(0,0),
		     &elemental_system->getfe()(0),
		     elemental_system->includesBodyForces());
      }
    }

    // use solution vector to update displacement dofs associated with locally-owned nodes
    void apfFEA::UpdateSolution(const double * sol) 
    {
      int num_components = apf::countComponents(apf_primary_field);
      apf::MeshEntity * mesh_ent = NULL;
      for(int ii = 0; ii < analysis_dim; ii++)
      {	
	for(apf::MeshIterator * it = apf_mesh->begin(ii); 
	    mesh_ent = apf_mesh->iterate(it);)
	{
	  if(apf_mesh->isOwned(mesh_ent))
	  {
	    apf::FieldShape * fs = apf::getShape(apf_primary_field);
	    int num_nodes = fs->countNodesOn(apf_mesh->getType(mesh_ent));
	    
	    for(int jj = 0; jj < num_nodes; jj++)
	    {
	      apf::Vector3 disp;
	      apf::getVector(apf_primary_field,mesh_ent,jj,disp);
	      
	      for(int kk = 0; kk < num_components; kk++)
	      {
		if(!apf::isFixed(apf_primary_numbering,mesh_ent,jj,kk))
		{
		  int global_number = getNumber(apf_primary_numbering,mesh_ent,jj,kk);
		  disp[kk] += sol[global_number - first_local_dof];
		}
	      }
	      apf::setVector(apf_primary_field,mesh_ent,jj,disp);
	    }
	  }
	}
      }
      apf::synchronize(apf_primary_field);
    }

    void apfFEA::WriteMesh(const std::string & nm)
    {
      apf::writeVtkFiles(nm.c_str(),apf_mesh);
    }
  }
}
