#include "amsiInterface.h"
#include "NonLinearElastic_UniformAdapt.h"
#include "Solvers.h"
#include <mpi.h>
#include <getopt.h>
#include <iostream>
int main (int argc, char ** argv)
{
  amsi::use_simmetrix = true;
  amsi::interfaceInit(argc,argv);
  Sim_logOn("simmetrix_log");
  pParMesh mesh = PM_load(argv[1],sthreadNone,NULL,NULL);
  amsi::NonLinElasticity * uniform_adapt =
    static_cast<amsi::NonLinElasticity*>(new amsi::UniformAdapt(MPI_COMM_WORLD,
                                                                NULL,
                                                                mesh));
  uniform_adapt->Adapt();
  uniform_adapt->WriteMesh(std::string("uniformly_adapted_mesh"));
  Sim_logOff();
  amsi::interfaceFree();
  return 0;
}
