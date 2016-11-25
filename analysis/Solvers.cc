#include "Solvers.h"
#include <cassert>
#include <iostream>
#include <iomanip>
#include <numeric>
#include <stdexcept>
#include <vector>
namespace amsi {
  void LinearSolver(FEA * fem,LAS * las)
  {
    int global_dof_count, local_dof_count, first_local_dof;
    fem->ApplyBC_Dirichlet();
    /// configure linear system
    fem->RenumberDOFs();
    fem->GetDOFInfo(global_dof_count,local_dof_count,first_local_dof);
    las->Reinitialize(local_dof_count,global_dof_count,first_local_dof);
    las->Zero();
    /// assemble linear system
    fem->Assemble(las);
    double nrm = 0.0;
    las->GetVectorNorm(nrm);
    /// solve linear system
    las->solve();
    /// update analysis with solution
    double * solution = NULL;
    las->GetSolution(solution);
    fem->UpdateDOFs(solution);
    //las->PrintSolution(std::cout);
  }
  void NewtonSolver(FEA * fem,
                    LAS * las,
                    int iteration_cap,
                    double epsilon,
                    double dt,
                    double & residual_norm)
  {
    int global_dof_count, local_dof_count, first_local_dof;
    int newton_iteration = 0;
    while(true)
    {
      std::cout << "Newton iteration " << newton_iteration << ":" << std::endl;
      fem->ApplyBC_Dirichlet();
      fem->RenumberDOFs();
      fem->GetDOFInfo(global_dof_count,local_dof_count,first_local_dof);
      las->Reinitialize(local_dof_count,global_dof_count,first_local_dof);
      las->Zero();
      fem->Assemble(las);
      double nrm = 0.0;
      las->GetVectorNorm(nrm);
      las->solve();
      double * solution = NULL;
      las->GetSolution(solution);
      fem->UpdateDOFs(solution);
      las->GetVectorNorm(residual_norm);
      if (residual_norm < epsilon)
        break;
      if (newton_iteration > iteration_cap)
        break;
      fem->Adapt();
      newton_iteration++;
    }
  }
}
