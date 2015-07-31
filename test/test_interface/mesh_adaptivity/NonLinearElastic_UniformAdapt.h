#ifndef NONLINEARELASTIC_UNIFORMADAPT_H_
#define NONLINEARELASTIC_UNIFORMADAPT_H_

#include <apfSIM.h>
#include <NonLinElasticity.h>

namespace amsi {

  class UniformAdapt : public Analysis::NonLinElasticity
  {
  public:
    UniformAdapt(MPI_Comm comm,
		 pGModel in_model,
		 pParMesh in_mesh) :
    FEA(comm,"uniform_adapt"),
    NonLinElasticity(comm,in_model,in_mesh)
    {
      mesh_size_field = apf::createSIMLagrangeField(apf_mesh,"mesh_size",apf::SCALAR,1);
    }
    
    virtual void Adapt()
    {
      assert(mesh_size_field);
      apf::MeshEntity * vert = NULL;
      for(apf::MeshIterator * it = apf_mesh->begin(0); vert = apf_mesh->iterate(it);)
      	apf::setScalar(mesh_size_field,vert,0,0.1);
      apfSimFEA::Adapt();
      WriteMesh("post-adapt");
    }
  };
  
}

#endif
