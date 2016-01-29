// only included if being built with simmetrix libraries
#ifndef SIMFEA_H_
#define SIMFEA_H_
#include "FEA.h"
#include "amsiInterface.h"
pAManager SModel_attManager(pModel model);
namespace amsi {
  namespace Analysis {
    void Model_GetClassifiedEntities(pMesh mesh,
                                     pGEntity entity,
                                     int dim,
                                     std::list<pEntity> & classified);
    void initAttributeCase(pGModel model,const std::string & attr_cs);
    void clearModelAttributes(pGModel mdl);
    bool requiresIncrementalLoading(pGModel mdl, const char * attr);
    bool hasAttribute(pGEntity entity, const char * attr);
    void getWithAttribute(pGModel mdl, const char * attr, std::list<pGEntity> & ents);
    class SimFEA : public virtual FEA
    {
    protected:
      const std::string model_name;
      const std::string mesh_name;
      pGModel model;
      pParMesh mesh;
      pMesh part;
      bool should_adapt;
      pField sim_size_field;
      pPList fields_to_map;
    public:
      SimFEA(MPI_Comm comm,
             const std::string & in_analysis_name,
             const std::string & in_model_name,
             pGModel in_model,
             const std::string & in_mesh_name,
             pParMesh in_mesh);
      SimFEA(MPI_Comm comm,
             const std::string & in_analysis_name,
             pGModel in_model,
             pParMesh in_mesh);
      SimFEA(const std::string & in_analysis_name,
             const std::string & in_model_name,
             pGModel in_model,
             const std::string & in_mesh_name,
             pParMesh in_mesh);
      SimFEA(const std::string & in_analysis_name,
             pGModel in_model,
             pParMesh in_mesh);
      std::string GetModelName() const {return model_name;}
      std::string GetMeshName() const {return mesh_name;}
      virtual void Adapt();
      virtual void addFieldToMap(pField);
      friend std::ostream& operator<<(std::ostream& out, const SimFEA & analysis);
    private:
    };
  }
}
#endif
