#include "test.h"
#include "amsiMultiscale.h"
int main(int argc, char ** argv)
{
  std::cerr << "MPI_COMM_WORLD" << MPI_COMM_WORLD << std::endl;
  amsi::MPI mpi(argc, argv, MPI_COMM_WORLD);
  amsi::MultiscaleOptions multiscale_options{.scales = {{"scale1", 1}}};
  amsi::Multiscale m(multiscale_options, mpi);
  return 0;
}
