#include "NonLinElasticity.h"
#include "NonLinElasticIntegrator.h"
#include "apfFunctions.h"
#include <apfSIM.h>
namespace amsi
{
  NonLinElasticity::NonLinElasticity(pGModel imdl,
                                     pParMesh imsh,
                                     pACase pd,
                                     pACase ss,
                                     MPI_Comm cm)
    : FEA(cm)
    , apfSimFEA(imdl,imsh,pd,ss,cm)
    , poisson_ratio(0.3)
    , youngs_modulus(20000)
  {
    if(!(apf_primary_field = apf_mesh->findField("displacement")))
    {
      apf_primary_field = apf::createSIMLagrangeField(apf_mesh,"displacement",apf::VECTOR,1);
      addFieldToMap(reinterpret_cast<pField>(apf_primary_field));
    }
    apf_primary_numbering = apf::createNumbering(apf_primary_field);
    elemental_system = new NonLinElasticIntegrator(apf_primary_field,1,youngs_modulus,poisson_ratio);
    if(pd)
    {
      int dir_tps[] = {amsi::FieldUnit::displacement};
      amsi::buildSimBCQueries(pd,amsi::BCType::dirichlet,&dir_tps[0],(&dir_tps[0])+1,std::back_inserter(dir_bcs));
      int neu_tps[] = {amsi::NeuBCType::traction,amsi::NeuBCType::pressure};
      amsi::buildSimBCQueries(pd,amsi::BCType::neumann,&neu_tps[0],(&neu_tps[0])+1,std::back_inserter(neu_bcs));
    }
    setSimulationTime(0.01);
  }
}
