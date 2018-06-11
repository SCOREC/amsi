#include "apfFEA.h"
#include "apfFunctions.h"
#include <apfShape.h>
#include <apfNumbering.h>
#include <maShape.h>
#include <cassert>
namespace amsi
{
  // assumes only linear tets
  apf::Field * analyzeMeshQuality(apf::Mesh * mesh, apf::Field * disp_field)
  {
    int analysis_dim = mesh->getDimension();
    apf::Field * f = createStepField(mesh,"quality",apf::SCALAR);
    apf::MeshEntity * me = NULL;
    for(apf::MeshIterator * it = mesh->begin(analysis_dim);
        (me = mesh->iterate(it));)
    {
      apf::Vector3 verts[4];
      apf::Adjacent v;
      mesh->getAdjacent(me,0,v);
      for(int ii = 0; ii < 4; ii++)
      {
        apf::Vector3 disp;
        apf::getVector(disp_field,v[ii],0,disp);
        mesh->getPoint(v[ii],0,verts[ii]);
        verts[ii] += disp;
      }
      apf::setScalar(f,me,0,ma::measureLinearTetQuality(verts));
    }
    return f;
  }
  void apfFEA::RenumberDOFs()
  {
    if(!numbered)
    {
      global_dof_count = 0;
      local_dof_count = apf::NaiveOrder(apf_primary_numbering);
      int analysis_size, analysis_rank;
      MPI_Comm_rank(analysis_comm,&analysis_rank);
      MPI_Comm_size(analysis_comm,&analysis_size);
      if(analysis_size - 1 == analysis_rank)
        local_dof_count += constraint_dofs;
      int dofs[analysis_size];
      memset(&dofs[0],0,analysis_size*sizeof(int));
      dofs[analysis_rank] = local_dof_count;
      MPI_Allgather(&local_dof_count,1,MPI_INTEGER,
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
        (me = apf_mesh->iterate(it));)
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
      apf::destroyMeshElement(melm);
    }
  }
  // use solution vector to update displacement dofs associated with locally-owned nodes
  void apfFEA::UpdateDOFs(const double * solution)
  {
    int num_components = apf::countComponents(apf_primary_field);
    apf::MeshEntity * mesh_ent = NULL;
    for(int ii = 0; ii < analysis_dim; ii++)
    {
      for(apf::MeshIterator * it = apf_mesh->begin(ii);
          (mesh_ent = apf_mesh->iterate(it));)
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
                disp[kk] += solution[global_number - first_local_dof];
              }
            }
            apf::setVector(apf_primary_field,mesh_ent,jj,disp);
          }
        }
      }
    }
    apf::synchronize(apf_primary_field);
  }
}
