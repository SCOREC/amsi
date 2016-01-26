#include "amsiInterface.h"
#include "amsiBCDirichlet.h"
#include "apfFEA.h"

#ifdef SIM
#  include "apfSIM.h"
#endif

#include "../../test.h"

#include <mpi.h>
#include <getopt.h>
#include <fstream>

std::string license_filename;
std::string model_filename;
std::string mesh_filename;
std::string petsc_filename;
std::string dirichlet_filename;

void display_help_string()
{
  std::cout << "Usage: dirichlet_test [OPTIONS]" << std::endl
	    << "[-h, --help]                      display this help text" << std::endl
#           ifdef SIM
	    << "[-l, --license path_to_license]   path to simmetrix license file" << std::endl
#           endif
	    << "[-g, --geom model_file]           the model file" << std::endl
	    << "[-m, --mesh mesh_file]            the mesh file" << std::endl
	    << "[-p, --petsc options_file]        the PETSc options file" << std::endl
	    << "[-d, --dirichlet]                 the dirichlet specification file" << std::endl;
}

bool parse_options(int argc, char ** argv)
{
  bool result = true;
  bool exit_loop = false;

  while(!exit_loop)
  {
    static struct option long_options[] =
    {
      {"help",      no_argument,       0, 'h'},
#     ifdef SIM      
      {"license",   required_argument, 0, 'l'},
#     endif
      {"model",     required_argument, 0, 'g'},
      {"mesh",      required_argument, 0, 'm'},
      {"petsc",     required_argument, 0, 'p'},
      {"dirichlet", required_argument, 0, 'd'}
    };

    int option_index = 0;
    int option = getopt_long(argc,argv, "h:l:g:m:p:d:",long_options,&option_index);
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
    case 'd':
      dirichlet_filename = optarg;
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


int main(int argc, char ** argv)
{
  int failed = 0;

  if(parse_options(argc,argv))
  {
    amsi::amsiInit(argc,argv);
    apf::Mesh * apf_mesh = NULL;
#   ifdef SIM
    pGModel model = GM_load(model_filename.c_str(),0,NULL);
    pParMesh mesh = PM_load(mesh_filename.c_str(),sthreadNone,model,NULL);
    apf_mesh = apf::createMesh(mesh);
#   else
    
#   endif

    amsi::Analysis::apfFEA fea(MPI_COMM_WORLD,apf_mesh);

    std::vector<amsi::DirichletSpecification> spec;
    std::fstream dirichlet_file(dirichlet_filename.c_str(), std::fstream::in);
    amsi::buildDirichletSpecFromStream(dirichlet_file,spec);

    fea.ApplyBC_Dirichlet(spec);

    
    

#   ifdef SIM

#   else
      
#   endif
    amsi::amsiFree();
  }

    
  return failed;
}
