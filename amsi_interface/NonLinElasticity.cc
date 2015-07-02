#include "NonLinElasticity.h"
#include "SurfaceTractionIntegrator.h"
#include "NonLinElasticIntegrator.h"
#include "apfFunctions.h"

namespace amsi
{
  namespace Analysis
  {

    NonLinElasticity::NonLinElasticity(MPI_Comm comm, pGModel in_model, pParMesh in_mesh) :
      FEA(comm,"nonlinear_elasticity"),
      apfSimFEA("nonlinear_elasticity",in_model,in_mesh),
      poisson_ratio(0.3),
      youngs_modulus(20000)
    {
      if(!(apf_primary_field = apf_mesh->findField("displacement")))
	apf_primary_field = apf::createLagrangeField(apf_mesh,"displacement",apf::VECTOR,1);
      apf_primary_numbering = apf::createNumbering(apf_primary_field);
      elemental_system = new NonLinElasticIntegrator(apf_primary_field,1,youngs_modulus,poisson_ratio);
      neumann_integrator = new SurfaceTractionIntegrator(apf_primary_field,1);
    }

    void NonLinElasticity::UpdateSolution(const double * solution)
    {
      apfFEA::UpdateSolution(solution);
      //displaceMesh(apf_primary_field);
    }
  }
}
