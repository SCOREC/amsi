#include "amsiMPI.h"
template <typename T>
MPI_Datatype mpi_type() {return MPI_BYTE;}
template <>
MPI_Datatype mpi_type<double>() {return MPI_DOUBLE;}
template <>
MPI_Datatype mpi_type<int>() {return MPI_INT;}

