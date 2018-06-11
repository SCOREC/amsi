#include "test.h"
#include "amsiMultiscale.h"
int main(int argc, char ** argv)
{
  amsi::initMultiscale(argc,argv, MPI_COMM_WORLD);
  amsi::freeMultiscale();
  return 0;
}
