#include "test.h"
#include "amsiUtil.h"
int main(int argc, char ** argv)
{
  amsi::utilInit(argc,argv);
  amsi::utilFree();
  return 0;
}
