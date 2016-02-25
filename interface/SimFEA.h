#ifndef SIMFEA_H_
#define SIMFEA_H_
#include "amsiFEA.h"
#include "sim.h"
#include "simBoundaryConditions.h"
namespace amsi
{
  void Model_GetClassifiedEntities(pMesh mesh,
                                   pGEntity entity,
                                   int dim,
                                   std::list<pEntity> & classified);
  class SimFEA : public virtual FEA
  {
  private:
    void init();
  protected:
    pGModel model;
    pParMesh mesh;
    pMesh part;
    bool should_adapt;
    pField sim_size_field;
    pPList fields_to_map;
  public:
    SimFEA(pGModel imdl,
           pParMesh imsh,
           pACase pd,
           MPI_Comm cm = AMSI_COMM_SCALE);
    pGModel getGeometricDomain() { return model; }
    virtual void Adapt();
    virtual void addFieldToMap(pField);
  };
}
#endif
