#include <amsiLinearElasticConstitutive.h>
#include <mpi.h>
int main(int argc, char * argv[])
{
  MPI_Init(&argc,&argv);
  apf::DynamicMatrix C = amsi::isotropicLinearElasticityTensor(10000,0.3);
  MPI_Finalize();
  return 0;
}
