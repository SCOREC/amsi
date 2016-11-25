#include "test.h"
#include "amsiMultiscale.h"
int main(int argc, char ** argv)
{
  amsi::initMultiscale(argc,argv);
  amsi::freeMultiscale();
  return 0;
}
