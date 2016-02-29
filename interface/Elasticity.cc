#include "Elasticity.h"
#include "LinearElasticIntegrator.h"
#include <vector>
#include <list>
#include <set>
#include <fstream>
#include <limits>
namespace amsi
{
  Elasticity::Elasticity(pGModel imdl,
                         pParMesh imsh,
                         pACase pd,
                         MPI_Comm cm,
                         double E, double v)
    : FEA(cm)
    , apfSimFEA(imdl,imsh,pd,cm)
    , youngs_modulus(E)
    , poisson_ratio(v)
  {
    apf_primary_field = apf::createLagrangeField(apf_mesh,"displacement",apf::VECTOR,1);
    apf_primary_numbering = apf::createNumbering(apf_primary_field);
    elemental_system = new LinearElasticIntegrator(apf_primary_field,1,youngs_modulus,poisson_ratio);
    int dir_tps[] = {amsi::DISPLACEMENT};
    amsi::buildSimBCQueries(pd,amsi::DIRICHLET,&dir_tps[0],(&dir_tps[0])+1,std::back_inserter(dir_bcs));
    int neu_tps[] = {amsi::SURFACE_TRACTION,amsi::NORMAL_PRESSURE};
    amsi::buildSimBCQueries(pd,amsi::NEUMANN,&neu_tps[0],(&neu_tps[0])+2,std::back_inserter(neu_bcs));
    //int fei_tps[] = {amsi::ISO_LIN_ELASTIC};
    //amsi::buildSimFeis(pd,&feis[0],(&feis[0])+1),std::back_inserter(feis));
    simulation_time = 1.0;
  }
}
