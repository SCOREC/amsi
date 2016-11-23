#include "test.h"
#include "amsiInterface.h"
int main(int argc, char ** argv)
{
  amsi::interfaceInit(argc,argv);
  amsi::interfaceFree();
  return 0;
}
