#include "amsiInterface.h"
#include "NonLinElasticity.h"
#include "Solvers.h"
#include <mpi.h>
#include <getopt.h>
#include <iostream>
// option parameters
std::string model_filename;
std::string mesh_filename;
void display_help_string ()
{
  std::cout << "Usage: linear_elasticity [OPTIONS]" << std::endl
            << "[-h, --help]                      display this help text" << std::endl
            << "[-g, --geom model_file]           the model file (.smd)" << std::endl
            << "[-m, --mesh mesh_file]            the mesh files (.sms)" << std::endl;
}
bool parse_options (int argc, char ** argv)
{
  bool result = true;
  bool exit_loop = false;
  while(!exit_loop)
  {
    static struct option long_options[] =
    {
      {"help", no_argument, 0, 'h'},
      {"model", required_argument, 0, 'g'},
      {"mesh", required_argument, 0, 'm'}
    };
    int option_index = 0;
    int option = getopt_long(argc,argv, "h:g:m:", long_options,&option_index);
    switch(option)
    {
    case 'h':
      display_help_string();
      result = false;
      break;
    case 'g':
      model_filename = optarg;
      break;
    case 'm':
      mesh_filename = optarg;
      break;
    case '?':
      display_help_string();
      result = false;
    case -1:
      exit_loop = true;
      break;
    }
  }
  return result;
}
using namespace amsi::Analysis;
int main (int argc, char ** argv)
{
  int result = 0;
  if(parse_options(argc,argv))
  {
    amsi::use_simmetrix = true;
    amsi::use_petsc = true;
    amsi::interfaceInit(argc,argv);
    Sim_logOn("simmetrix_log");
    pGModel model = GM_load(model_filename.c_str(),0,NULL);
    amsi::initAttributeCase(model,"constraints");
    pParMesh mesh = PM_load(mesh_filename.c_str(),sthreadNone,model,NULL);
    LAS * linear_system = static_cast<LAS*>(new PetscLAS(0,0));
    NonLinElasticity * nonlinear_elasticity = new NonLinElasticity(MPI_COMM_WORLD,
                                                                   model,
                                                                   mesh);
    double residual_norm = 0.0;
    NewtonSolver(nonlinear_elasticity,linear_system,30,1e-8,1.0,residual_norm);
    nonlinear_elasticity->WriteMesh(std::string("isotropic_nonlinear_elastic_result"));
    Sim_logOff();
    amsi::interfaceFree();
  }
  else result++;
  return result;
}
