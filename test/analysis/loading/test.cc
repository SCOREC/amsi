#include "amsiAnalysis.h"
#include <cassert>
int main(int argc, char * argv[])
{
  assert(argc == 3);
  int result = 0;
  amsi::use_simmetrix = true;
  amsi::initAnalysis(argc,argv);
  Sim_logOn("simmetrix_log");
  pGModel mdl = GM_load(argv[1],NULL,NULL);
  result += mdl == NULL;
  if(mdl)
  {
    pParMesh msh = PM_load(argv[2],sthreadNone,mdl,NULL);
    result += msh == NULL;
  }
  Sim_logOff();
  amsi::freeAnalysis();
  return result;
}
