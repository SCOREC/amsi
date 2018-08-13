#include "SimFEA.h"
#include <cassert>
namespace amsi
{
  template <>
  void Model_PrintInfo<pGModel>(pGModel, std::ostream &)
  { }
  template <>
  void Mesh_PrintInfo<pMesh>(pMesh mesh, std::ostream & out)
  {
    out << "The specified mesh or mesh-part has: " << std::endl;
    out << "\t" << M_numRegions(mesh) << " regions." << std::endl;
    out << "\t" << M_numFaces(mesh) << " faces." << std::endl;
    out << "\t" << M_numEdges(mesh) << " edges." << std::endl;
    out << "\t" << M_numVertices(mesh) << " vertices." << std::endl;
  }
  SimFEA::SimFEA(pGModel imdl,
                 pParMesh imsh,
                 pACase pd,
                 pACase ss,
                 MPI_Comm cm)
    : FEA(cm)
    , model(imdl)
    , mesh(imsh)
    , part(PM_mesh(imsh,0))
    , should_adapt(false)
    , prob_def(pd)
    , solution_strategy(ss)
    , sim_size_field()
    , fields_to_map(PList_new())
  {  }
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
