#include "amsiAnalysis.h"
#include <SimModel.h>
#include <cassert>
#include <cstdlib>
int main(int argc, char * argv[])
{
  assert(argc == 3);
  int result = 0;
  std::string lic = std::getenv("SIM_LICENSE_FILE");
  amsi::useSimmetrix(lic);
  amsi::initAnalysis(argc,argv, MPI_COMM_WORLD);
  Sim_logOn("simmetrix_log");
  pGModel mdl = GM_load(argv[1],NULL,NULL);
  result += mdl == NULL;
  if(mdl)
  {
    pParMesh msh = PM_load(argv[2],mdl,NULL);
    result += msh == NULL;
  }
  Sim_logOff();
  amsi::freeAnalysis();
  return result;
}
