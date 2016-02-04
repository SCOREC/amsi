#ifndef FEA_H_
#define FEA_H_

// Linear Solver Interface
#include "LAS.h"

// Standard Libs
#include <cstring> // memset
#include <iostream>
#include <list>
#include <map>

#include <mpi.h>

namespace amsi {
  namespace Analysis {

    // TODO: replace with functor subclassed on field library
    bool isFixed(int);

    template <typename T>
      void Model_PrintInfo(T model, std::ostream & out);

    template <typename T>
      void Mesh_PrintInfo(T mesh, std::ostream & out);
    class FEA {
    protected:
      // the number of dofs on this processor
      int local_dof_count;

      // the first locally-owned dof (using the global numbering)
      int first_local_dof;

      // the total number of dofs in the analysis
      int global_dof_count;

      // the number of fixed locally-fixed dofs
      int fixed_dofs;

      // whether the dofs have been numbered or not
      bool numbered;

      // the current simulation time (for dynamics) or load percentage (for statics)
      double simulation_time;
      
      // name of this analysis
      std::string name;

      // dimensionality of the analysis
      int analysis_dim;
      
      // mpi communicator containing only those processes undertaking this analysis
      MPI_Comm analysis_comm;

    public:
      FEA(MPI_Comm comm, const std::string & analysis_name);
     
      std::string GetName() const { return name; }

      virtual void Adapt() {};
      virtual void ApplyBC_Dirichlet() {};
      virtual void ApplyBC_Neumann(LAS *) {};
      virtual void Assemble(LAS *) {};
      
      template <typename NODE_TYPE>
	void AssembleDOFs(LAS * las, 
			  int num_elemental_dofs,
			  int * dof_numbers, 
			  const NODE_TYPE * node_values, 
			  double * Ke,
			  double * fe,
			  bool includes_body_forces) const;
      
      virtual void GetDOFInfo(int& global,int& local,int& offset)
      {
	global = global_dof_count;
	local = local_dof_count;
	offset = first_local_dof;
      };

      virtual void RenumberDOFs() {};
      virtual void UpdateDOFs(const double * sol) {};
    private:
    };


    template <typename NODE_TYPE>
      void FEA::AssembleDOFs(LAS * las,
			     int num_elemental_dofs,
			     int * dof_numbers,
			     const NODE_TYPE * node_values,
			     double * Ke,
			     double * fe,
			     bool includes_body_forces) const
    {
      if(Ke != NULL)
      {
	if(!includes_body_forces)
	{
	  double * bf = new double[num_elemental_dofs]();
	  for(int ii = 0; ii < num_elemental_dofs; ii++)
	  {
	    const int & global_i = dof_numbers[ii];
	    if(!isFixed(global_i))
	    {
	      for(int jj = 0; jj < num_elemental_dofs; jj++)
	      {
		const double & val = Ke[ii * num_elemental_dofs + jj];
		double j_val = node_values[jj / analysis_dim][jj % analysis_dim];
		if(j_val != 0.0)
		  bf[ii] += -val * j_val;
	      }
	    }
	  }
	  las->AddToVector(num_elemental_dofs,dof_numbers,&bf[0]);
	  delete [] bf;
	}
       
	las->AddToMatrix(num_elemental_dofs,dof_numbers,
			 num_elemental_dofs,dof_numbers,
			 &Ke[0]);
      }
      las->AddToVector(num_elemental_dofs,dof_numbers,&fe[0]);
    }
   
  } // namespace Core
} // namespace SCOREC
#endif




