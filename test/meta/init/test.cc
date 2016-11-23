#include "test.h"
#include "amsiControl.h"
int main(int argc, char ** argv)
{
  amsi::controlInit(argc,argv);
  amsi::controlFree();
  return 0;
}
