#include "NonLinElasticity.h"
#include "NonLinElasticIntegrator.h"
#include "apfFunctions.h"
#include <apfSIM.h>
namespace amsi
{
  NonLinElasticity::NonLinElasticity(pGModel imdl,
                                     pParMesh imsh,
                                     pACase pd,
                                     MPI_Comm cm)
    : FEA(cm)
    , apfSimFEA(imdl,imsh,pd,cm)
    , poisson_ratio(0.3)
    , youngs_modulus(20000)
  {
    if(!(apf_primary_field = apf_mesh->findField("displacement")))
    {
      apf_primary_field = apf::createSIMLagrangeField(apf_mesh,"displacement",apf::VECTOR,1);
      addFieldToMap(apf_primary_field);
    }
    apf_primary_numbering = apf::createNumbering(apf_primary_field);
    elemental_system = new NonLinElasticIntegrator(apf_primary_field,1,youngs_modulus,poisson_ratio);
    if(pd)
    {
      int dir_tps[] = {amsi::DISPLACEMENT};
      amsi::buildSimBCQueries(pd,amsi::DIRICHLET,&dir_tps[0],(&dir_tps[0])+1,std::back_inserter(dir_bcs));
      int neu_tps[] = {amsi::SURFACE_TRACTION,amsi::NORMAL_PRESSURE};
      amsi::buildSimBCQueries(pd,amsi::NEUMANN,&neu_tps[0],(&neu_tps[0])+1,std::back_inserter(neu_bcs));
    }
    setSimulationTime(0.01);
  }
}
