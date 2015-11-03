#include "amsiMPI.h"

double distributedAverage(double partial_sum, int partial_count, MPI_Comm cm)
{
  double sum = 0.0;
  MPI_Allreduce(&partial_sum,&sum,1,MPI_DOUBLE,MPI_SUM,cm);
  int count = 0;
  MPI_Allreduce(&partial_count,&count,1,MPI_INTEGER,MPI_SUM,cm);
  return sum / count;
}
