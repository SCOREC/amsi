#include "ControlService.h"
#include "amsi.h"
using namespace amsi;
int main(int argc, char * argv[])
{
  initializer = new amsiInit;
  amsiInit(argc,argv);
  ProcessSet * macro_ps = static_cast<ProcessSet*>(new ProcessSet_T<std::pair<int,int> >(0,128));
  ProcessSet * micro_ps = static_cast<ProcessSet*>(new ProcessSet_T<std::pair<int,int> >(129,16256));
  Task macro(macro_ps);
  Task micro(micro_ps);

  
}
