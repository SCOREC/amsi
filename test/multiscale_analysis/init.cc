#include "test.h"
#include "amsiMultiscale.h"
#include "amsiAnalysis.h"
int main(int argc, char * argv[])
{
  amsi::initMultiscale(argc,argv);
  amsi::initAnalysis(argc,argv);
  amsi::freeAnalysis();
  amsi::freeMultiscale();
  return 0;
}
