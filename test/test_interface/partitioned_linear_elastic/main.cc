#include "Elasticity.h"
#include "Solvers.h"

#include <amsiInterface.h>
#include <amsiMPI.h>

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
    amsi::initializer = new amsi::amsiInterfaceInit;
    amsi::use_petsc = true;
    amsi::use_simmetrix = true;
    amsi::amsiInit(argc,argv);
    std::cout << "3rd-party libraries initialized, reading simulation input files:" << std::endl;

    pGModel model = GM_load(model_filename.c_str(),0,NULL);
    std::string cnstrnts("constraints");
    initAttributeCase(model,cnstrnts);
    pParMesh mesh = PM_load(mesh_filename.c_str(),sthreadNone,model,NULL);

    std::cout << "Input files loaded, initializing analysis: " << std::endl;
    LAS * linear_system = static_cast<LAS*>(new PetscLAS(0,0));
    
    Elasticity * isotropic_linear_elasticity = new Elasticity(AMSI_COMM_LOCAL,
							      model,
							      mesh,
							      43200000,0.23);

    std::cout << "Analysis objects created, commencing analysis: " << std::endl;
    LinearSolver(isotropic_linear_elasticity,linear_system);
    std::cout << "Analysis complete, writing results file(s):" << std::endl;

    isotropic_linear_elasticity->WriteMesh(std::string("isotropic_linear_elastic_result"));
    std::cout << "Results file(s) written, shutting down 3rd-party libs" << std::endl;

    amsi::amsiFree();
    std::cout << "3rd-party libraries shut down, exiting analysis.." << std::endl;
  }
  else result--;
  return result;
}
