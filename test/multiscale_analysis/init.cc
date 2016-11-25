#include "test.h"
#include "amsiMultiscale.h"
#include "amsiAnalysis.h"
int main(int argc, char * argv[])
{
  amsi::initMultiscale();
  amsi::initAnalysis();
  amsi::freeAnalysis();
  amsi::freeMultiscale();
  return 0;
}
