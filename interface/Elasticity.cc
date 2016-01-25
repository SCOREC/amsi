/****************************************************************************** 
(c) 2015 Scientific Computation Research Center, 
         Rensselaer Polytechnic Institute. All rights reserved.
  
  The LICENSE-SCOREC file included with this distribution describes the terms
  of the SCOREC Non-Commercial License this program is distributed under.
*******************************************************************************/

#include "Elasticity.h"
#include "LinearElasticIntegrator.h"
#include "SurfaceTractionIntegrator.h"

#include <vector>
#include <list>
#include <set>
#include <fstream>
#include <limits>

namespace amsi {
  namespace Analysis {

    Elasticity::Elasticity(MPI_Comm comm,
			   pGModel in_model,
			   pParMesh in_mesh,
			   double E, double v) :
      FEA(comm,"linear_elasticity"),
      apfSimFEA(comm,
		"linear_elasticity",
		in_model,
		in_mesh),
      youngs_modulus(E),
      poisson_ratio(v)
    {
      apf_primary_field = apf::createLagrangeField(apf_mesh,"displacement",apf::VECTOR,1);
      apf_primary_numbering = apf::createNumbering(apf_primary_field);
      elemental_system = new LinearElasticIntegrator(apf_primary_field,1,youngs_modulus,poisson_ratio);
      neumann_integrator = new SurfaceTractionIntegrator(apf_primary_field,1);      
    }
  }
}



