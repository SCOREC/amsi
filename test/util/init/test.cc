#include "test.h"
#include "amsiUtil.h"

int main(int argc, char ** argv)
{
  amsi::initUtil(argc,argv, MPI_COMM_WORLD);
  amsi::freeUtil();
  return 0;
}
