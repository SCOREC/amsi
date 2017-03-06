// in-build
#include "../../test.h"
// in-project
#include "amsiDataDistribution.h"
// standard
#include <iostream>
#include <utility>
int main(int argc, char * argv[])
{
  using namespace amsi;
  MPI_Init(&argc,&argv);
  int sz = 0;
  int rnk = -1;
  MPI_Comm_size(MPI_COMM_WORLD,&sz);
  MPI_Comm_size(MPI_COMM_WORLD,&rnk);
  DataDistribution dd(sz,rnk);
  int failed = 0;
  MPI_Finalize();
  return failed;
}
