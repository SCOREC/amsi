// in-build
#include "../../test.h"

// in-project
#include "amsiMPI.h"
#include "amsiByteStream.h"

// standard
#include <iostream>
#include <utility>

//using namespace AMSI;

int main(int argc, char * argv[])
{
  int result = 0;
  MPI_Init(&argc,&argv);

  int rank = -1;
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);

  int size;
  MPI_Comm_size(MPI_COMM_WORLD,&size);

  int int_val = 6;
  double double_val = 3.14;

  void * data = talloc(int_val,double_val);
  byte_pack(data,int_val,double_val);

  int recovered_int = -1;
  double recovered_double = 0.0;

  byte_unpack(data,recovered_int,recovered_double);
  free(data);

//  result += strcmp(char_string,recovered_string);
  result += test("recover integer",int_val,recovered_int);
  result += test("recover double",double_val,recovered_double);

  std::cout << result << std::endl;

  MPI_Finalize();
  return result;
}
