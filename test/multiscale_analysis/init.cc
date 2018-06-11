#include "test.h"
#include "amsiMultiscale.h"
#include "amsiAnalysis.h"
int main(int argc, char * argv[])
{
  amsi::initMultiscale(argc,argv,MPI_COMM_WORLD);
  amsi::initAnalysis(argc,argv,MPI_COMM_WORLD);
  amsi::freeAnalysis();
  amsi::freeMultiscale();
  return 0;
}
