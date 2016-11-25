#include "test.h"
#include "amsiMeta.h"
int main(int argc, char ** argv)
{
  amsi::initMultiscale(argc,argv);
  amsi::freeMultiscale();
  return 0;
}
