#ifndef SIMFEA_H_
#define SIMFEA_H_
#include "amsiFEA.h"
#include "sim.h"
#include "simBoundaryConditions.h"
namespace amsi {
  void Model_GetClassifiedEntities(pMesh mesh, pGEntity entity, int dim,
                                   std::list<pEntity>& classified);
  class SimFEA : public FEA {
    private:
    void init();

    protected:
    pGModel model;
    pParMesh mesh;
    pMesh part;
    bool should_adapt;
    pACase prob_def;
    pACase solution_strategy;
    pField sim_size_field;
    pPList fields_to_map;

    public:
    SimFEA(pGModel imdl, pParMesh imsh, pACase pd, pACase ss,
           MPI_Comm cm = AMSI_COMM_SCALE);
    pGModel getGeometricDomain() { return model; }
    pMesh getPart() { return part; }
    virtual void Adapt() override;
    virtual void addFieldToMap(pField);
    //virtual void RenumberDOFs() override;
    //virtual void Assemble(LAS*) override;
    //virtual void UpdateDOFs(const double*) override;
    virtual void ApplyBC_Dirichlet() override;
    virtual void ApplyBC_Neumann(LAS * las) override;
  };
}  // namespace amsi
#endif
