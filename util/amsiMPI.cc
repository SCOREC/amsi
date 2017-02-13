#include "amsiMPI.h"
MPI_Comm AMSI_COMM_WORLD;
MPI_Comm AMSI_COMM_SCALE;
namespace amsi
{
  template <typename T>
  MPI_Datatype mpi_type(T) {return MPI_BYTE;}
  template <>
  MPI_Datatype mpi_type<char>(char) {return MPI_CHAR;}
  template <>
  MPI_Datatype mpi_type<const char>(const char) {return MPI_CHAR;}
  template <>
  MPI_Datatype mpi_type<double>(double) {return MPI_DOUBLE;}
  template <>
  MPI_Datatype mpi_type<int>(int) {return MPI_INT;}
  template <>
  MPI_Datatype mpi_type<bool>(bool) {return MPI::BOOL;}
  template <>
  MPI_Datatype mpi_type<unsigned long>(unsigned long) {return MPI_UNSIGNED_LONG;}
}
