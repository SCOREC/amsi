#ifndef APFSIMFEA_H_
#define APFSIMFEA_H_
#include "apfFEA.h"
#include "SimFEA.h"
#include "SimTensorFieldQuery.h"
namespace amsi {
  class apfSimFEA : public apfFEA, public SimFEA
  {
  protected:
    apf::Field * mesh_size_field;
    bool mesh_adapted;
    int Entity_ApplyBC_Dirichlet(GEntity*,int);
    void Entity_ApplyBC_Neumann(LAS *,GEntity*,int);
    TensorFieldQuery * getForceOn(pGEntity mdl_ent);
  public:
    apfSimFEA(MPI_Comm comm,
              const std::string & in_analysis_name,
              pGModel in_model,
              pParMesh in_mesh);
    apfSimFEA(const std::string & in_analysis_name,
              const std::string & in_model_name,
              pGModel in_model,
              const std::string & in_mesh_name,
              pParMesh in_mesh);
    apfSimFEA(const std::string & in_analysis_name,
              pGModel in_model,
              pParMesh in_mesh);
    virtual void Adapt();
    virtual void addFieldToMap(apf::Field *);
    void ApplyBC_Dirichlet();
    virtual void ApplyBC_Neumann(LAS * las);
    void Assemble(LAS * las);
  private:
  };
}
#endif
