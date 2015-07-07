/******************************************************************************

  (c) 2015 Scientific Computation Research Center,
      Rensselaer Polytechnic Institute. All rights reserved.

  The LICENSE-SCOREC file included with this distribution describes the terms
  of the SCOREC Non-Commercial License this program is distributed under.

*******************************************************************************/

#ifndef APFFEA_H_
#define APFFEA_H_

#include "FEA.h"
#include "amsiBCDirichlet.h"
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

      // these are only here because they have not been properly abstracted into the FEA class, right now they are  apf::Integrator-s
      ElementalSystem * elemental_system;
      NeumannIntegrator * neumann_integrator;

      int Entity_ApplyBC_Dirichlet(apf::ModelEntity*,int,double);
      //void Entity_ApplyBC_Neumann(LAS*,apf::ModelEntity*,int);

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
      void ApplyBC_Dirichlet(std::vector<DirichletSpecification>&);
      //void ApplyBC_Neumann(LAS*);
      
      void Assemble(LAS*);
      
      void UpdateDOFs(const double*);
      
      void WriteMesh(const std::string &);
    };
  }
}

#endif
