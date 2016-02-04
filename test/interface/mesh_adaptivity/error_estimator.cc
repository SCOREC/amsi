#include "amsiInterface.h"
#include "NonLinearElastic_UniformAdapt.h"
#include "Solvers.h"
#include <mpi.h>
#include <getopt.h>
#include <iostream>
using namespace amsi::Analysis;
int main (int argc, char ** argv)
{
  amsi::use_simmetrix = true;
  amsi::use_petsc = true;
  amsi::interfaceInit(argc,argv);
  Sim_logOn("simmetrix_log");
  pGModel model = GM_load(argv[1],0,NULL);
  amsi::initAttributeCase(model,"constraints");
  pParMesh mesh = PM_load(argv[2],sthreadNone,model,NULL);
  LAS * linear_system = static_cast<LAS*>(new PetscLAS(0,0));
  NonLinElasticity * uniform_adapt =
    static_cast<NonLinElasticity*>(new amsi::UniformAdapt(MPI_COMM_WORLD,
                                                          model,
                                                          mesh));
  double residual_norm = 0.0;
  //currently fails during the second adaptation when retreiving dofgroups
  //NewtonSolver(uniform_adapt,linear_system,30,1e-8,1.0,residual_norm);
  uniform_adapt->WriteMesh(std::string("error_estimation"));
  Sim_logOff();
  amsi::interfaceFree();
  return 0;
}
