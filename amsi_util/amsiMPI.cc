#include "amsiMPI.h"
namespace amsi
{
template <typename T>
MPI_Datatype mpi_type(T) {return MPI_BYTE;}
template <>
MPI_Datatype mpi_type<double>(double) {return MPI_DOUBLE;}
template <>
MPI_Datatype mpi_type<int>(int) {return MPI_INT;}
template <>
MPI_Datatype mpi_type<bool>(bool) {return MPI::BOOL;}
}
