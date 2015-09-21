#include "amsiInterface.h"
#include "NonLinearElastic_UniformAdapt.h"
#include "Solvers.h"

#include <mpi.h>

#include <getopt.h>
#include <iostream>

// option parameters
std::string model_filename;
std::string mesh_filename;
std::string license_filename;
std::string petsc_filename;

void display_help_string ()
{
  std::cout << "Usage: linear_elasticity [OPTIONS]" << std::endl
	    << "[-h, --help]                      display this help text" << std::endl
	    << "[-l, --lincense path_to_license]  path to directory containing Simmetrix license" << std::endl
	    << "[-g, --geom model_file]           the model file (.smd)" << std::endl
	    << "[-m, --mesh mesh_file]            the mesh files (.sms)" << std::endl
	    << "[-p, --petsc options_file]        the PETSc options file" << std::endl;
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
      {"license", required_argument, 0, 'l'},
      {"model", required_argument, 0, 'g'},
      {"mesh", required_argument, 0, 'm'},
      {"petsc", required_argument, 0, 'p'}
    };

    int option_index = 0;
    int option = getopt_long(argc,argv, "h:l:g:m:p:", long_options,&option_index);
    
    switch(option)
    {
    case 'h':
      display_help_string();
      result = false;
      break;
    case 'l':
      license_filename = optarg;
      break;
    case 'g':
      model_filename = optarg;
      break;
    case 'm':
      mesh_filename = optarg;
      break;
    case 'p':
      petsc_filename = optarg;
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
  std::cout << "Attempting to read required command-line parameters: " << std::endl;
  if(parse_options(argc,argv))
  {
    std::cout << "Command-line parameters successfully parsed, initializing 3rd party libraries:" << std::endl;
    amsi::amsiInit(argc,argv);
    Sim_logOn("simmetrix_log");
    std::cout << "3rd-party libraries initialized, reading simulation input files:" << std::endl;

    pGModel model = GM_load(model_filename.c_str(),0,NULL);
    std::vector<std::string> attribute_names;
    attribute_names.push_back(std::string("constraints"));
    InitModelAttributes(model,attribute_names);
    pParMesh mesh = PM_load(mesh_filename.c_str(),sthreadNone,model,NULL);

    std::cout << "Input files loaded, initializing analysis: " << std::endl;
    LAS * linear_system = static_cast<LAS*>(new PetscLAS(0,0));

    NonLinElasticity * uniform_adapt =
      static_cast<NonLinElasticity*>(new amsi::UniformAdapt(MPI_COMM_WORLD,
							    model,
							    mesh));
    
    std::cout << "Analysis objects created, commencing analysis: " << std::endl;
    double residual_norm = 0.0;
    NewtonSolver(uniform_adapt,linear_system,30,1e-8,1.0,residual_norm);
    std::cout << "Analysis converged with residual norm " << residual_norm << ", writing results file(s):" << std::endl;

    uniform_adapt->WriteMesh(std::string("isotropic_nonlinear_elastic_result"));
    std::cout << "Results file(s) written, shutting down 3rd-party libs" << std::endl;

    Sim_logOff();
    amsi::amsiFree();
    std::cout << "3rd-party libraries shut down, exiting analysis.." << std::endl;
  }
  else result--;
  
  return result;
}
