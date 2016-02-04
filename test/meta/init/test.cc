#include "test.h"
#include "amsiMeta.h"
int main(int argc, char ** argv)
{
  amsi::metaInit(argc,argv);
  amsi::metaFree();
  return 0;
}
