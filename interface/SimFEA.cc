#include "SimFEA.h"
#include <cassert>
namespace amsi
{
  namespace Analysis
  {
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
