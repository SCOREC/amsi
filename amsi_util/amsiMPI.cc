#include "amsiMPI.h"
template <>
MPI_Datatype mpi_type<double>() {return MPI_DOUBLE;}
template <>
MPI_Datatype mpi_type<int>() {return MPI_INT;}
