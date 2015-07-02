// only included if being built with simmetrix libraries

#ifndef SIMFEA_H_
#define SIMFEA_H_

#include "FEA.h"
#include "amsiInterface.h"

//void P_setPos(pPoint,double,double,double);
pAManager SModel_attManager(pModel model);

namespace amsi {
  namespace Analysis {

    void Model_GetClassifiedEntities(pMesh mesh,
				     pGEntity entity,
				     int dim,
				     std::list<pEntity> & classified);

    void InitModelAttributes(pGModel model,
			     std::vector<std::string> & attribute_names);

    class SimFEA : public virtual FEA
    {
    protected:
      const std::string model_name;
      const std::string mesh_name;
      
      pGModel model;
      pParMesh mesh;
      pMesh part;
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

      friend std::ostream& operator<<(std::ostream& out, const SimFEA & analysis);
    private:
    };



  }
}



#endif
