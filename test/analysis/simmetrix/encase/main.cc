#include "sim.h"
#include "SimAdvModel.h"
#include "SimParasolidKrnl.h"
#include <cstdlib>
#include <iostream>
#include <sstream>
int main(int argc, char* argv[])
{
  MPI_Init(&argc,&argv);
  if(argc != 8)
  {
    std::cout << "Usage: " << argv[0] << " model_file x_double y_double z_double x_length y_length z_length" << std::endl;
    return -1;
  }
  Sim_logOn("sim.log");
  Sim_readLicenseFile("/net/common/meshSim/license/license.txt");
  SimModel_start();
  SimParasolid_start(1);
  pGModel mdl = GM_load(argv[1],0,NULL);
  double prms[6]{};
  for(int ii = 2; ii < argc; ++ii)
  {
    prms[ii-2] = atof(argv[ii]);
    std::cout << prms[ii-2] << " ";
  }
  std::cout << std::endl;
  GM_addBoxToModel(mdl,&prms[0],prms[3],prms[4],prms[5],NULL,NULL);
  std::stringstream nm;
  nm << "encased_" << argv[1];
  GM_write(mdl,nm.str().c_str(),0,NULL);
  GM_release(mdl);
  SimParasolid_stop(1);
  SimModel_stop();
  Sim_logOff();
  MPI_Finalize();
}
