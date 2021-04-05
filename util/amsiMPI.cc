#include "amsiMPI.h"
MPI_Comm AMSI_COMM_WORLD;
MPI_Comm AMSI_COMM_SCALE;
namespace amsi
{
  template <typename T>
  MPI_Datatype mpi_type(T) {return MPI_BYTE;}
  template <typename T>
  MPI_Datatype mpi_type() {return MPI_BYTE;}
  template <>
  MPI_Datatype mpi_type<char>() {return MPI_CHAR;}
  template <>
  MPI_Datatype mpi_type<const char>(const char) {return MPI_CHAR;}
  template <>
  MPI_Datatype mpi_type<const char>() {return MPI_CHAR;}
  template <>
  MPI_Datatype mpi_type(char) {return MPI_CHAR;}
  template <>
  MPI_Datatype mpi_type<double>() {return MPI_DOUBLE;}
  template <>
  MPI_Datatype mpi_type(double) {return MPI_DOUBLE;}
  template <>
  MPI_Datatype mpi_type<int>() {return MPI_INT;}
  template <>
  MPI_Datatype mpi_type(int) {return MPI_INT;}
  template <>
  MPI_Datatype mpi_type<bool>() { return MPI_CXX_BOOL;}
  template <>
  MPI_Datatype mpi_type(bool) { return MPI_CXX_BOOL;}
  template <>
  MPI_Datatype mpi_type<unsigned long>() {return MPI_UNSIGNED_LONG;}
  template <>
  MPI_Datatype mpi_type(unsigned long) {return MPI_UNSIGNED_LONG;}
}
