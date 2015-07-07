/******************************************************************************

  (c) 2015 Scientific Computation Research Center,
      Rensselaer Polytechnic Institute. All rights reserved.

  The LICENSE-SCOREC file included with this distribution describes the terms
  of the SCOREC Non-Commercial License this program is distributed under.

*******************************************************************************/

#ifndef APFFEA_H_
#define APFFEA_H_

#include "FEA.h"
#include "ElementalSystem.h"
#include "NeumannIntegrator.h"

#include <apf.h>
#include <apfMesh.h>
#include <apfNumbering.h>

namespace amsi {
  namespace Analysis {

    class apfFEA : public virtual FEA
    {
    protected:
      apf::Mesh * apf_mesh;
      apf::Field * apf_primary_field;
      apf::Numbering * apf_primary_numbering;

      // this is only here because it has not been properly abstracted into the FEA class, right now it is an apf::Integrator
      ElementalSystem * elemental_system;
      NeumannIntegrator * neumann_integrator;

    public:
    apfFEA() : 
      FEA(MPI_COMM_WORLD,"[apf_fea]"),
        apf_primary_field(),
        apf_mesh(),
        apf_primary_numbering(),
	elemental_system(),
	neumann_integrator()
        {};

    apfFEA(MPI_Comm comm) :
      FEA(comm,"[apf_fea]"),
	apf_primary_field(),
	apf_mesh(),
	apf_primary_numbering(),
	elemental_system(),
	neumann_integrator()
	{};

    apfFEA(MPI_Comm comm,
	   apf::Mesh * in_mesh) :
      FEA(comm,"[apf_fea]"),
	apf_primary_field(),
	apf_mesh(in_mesh),
	apf_primary_numbering(),
	elemental_system(),
	neumann_integrator()
	{
	  analysis_dim = apf_mesh->getDimension();
	};

      void RenumberDOFs();
      void Assemble(LAS * las);
      void UpdateSolution(const double*);
      void WriteMesh(const std::string &);
    };

    
    
  }
}

#endif
