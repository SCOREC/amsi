#include "amsiElasticityFEA.h"
#include "amsiLinearElasticConstitutive.h"
#include <vector>
#include <list>
#include <set>
#include <fstream>
#include <limits>
namespace amsi
{
  ElasticityFEA::ElasticityFEA(pGModel imdl, pParMesh imsh, pACase pd, MPI_Comm cm, double E, double v)
    : FEA(cm)
    , apfSimFEA(imdl,imsh,pd,cm)
    , youngs_modulus(E)
    , poisson_ratio(v)
  {
    // create fields and numberings
    apf_primary_field = apf::createLagrangeField(apf_mesh,"displacement",apf::VECTOR,1);
    apf_primary_numbering = apf::createNumbering(apf_primary_field);
    // create elemental systems
    elemental_system = new LinearElasticIntegrator(apf_primary_field,1,youngs_modulus,poisson_ratio);
    // create boundary conditions on existing fields
    std::vector<int> app_dir;
    getApplicableBCTypesForField(amsi::FieldUnit::displacement,amsi::BCType::dirichlet,std::back_inserter(app_dir));
    amsi::buildSimBCQueries(pd,amsi::BCType::dirichlet,app_dir.begin(),app_dir.end(),std::back_inserter(dir_bcs));
    std::vector<int> app_neu;
    getApplicableBCTypesForField(amsi::FieldUnit::displacement,amsi::BCType::neumann,std::back_inserter(app_neu));
    amsi::buildSimBCQueries(pd,amsi::BCType::neumann,app_neu.begin(),app_neu.end(),std::back_inserter(neu_bcs));
    setSimulationTime(1.0);
  }
}
