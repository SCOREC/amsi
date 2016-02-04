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
  amsi::interfaceInit(argc,argv);
  Sim_logOn("simmetrix_log");
  pParMesh mesh = PM_load(argv[1],sthreadNone,NULL,NULL);
  NonLinElasticity * uniform_adapt =
    static_cast<NonLinElasticity*>(new amsi::UniformAdapt(MPI_COMM_WORLD,
                                                          NULL,
                                                          mesh));
  uniform_adapt->Adapt();
  uniform_adapt->WriteMesh(std::string("uniformly_adapted_mesh"));
  Sim_logOff();
  amsi::interfaceFree();
  return 0;
}
