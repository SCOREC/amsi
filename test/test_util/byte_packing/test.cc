// in-build
#include "../../test.h"

// in-project
#include "amsiMPI.h"
#include "amsiByteStream.h"

// standard
#include <iostream>
#include <utility>

//using namespace AMSI;

struct pod_struct {
  int int_val;
  char char_val;
  double double_val;
};

int main(int argc, char * argv[])
{
  int result = 0;

  int int_val = 6;
  double double_val = 3.14;

  void * data = talloc(int_val,double_val);
  byte_pack(data,int_val,double_val);

  int recovered_int = -1;
  double recovered_double = 0.0;

  byte_unpack(data,recovered_int,recovered_double);
  free(data);

  pod_struct struct_val;
  struct_val.int_val = 16;
  struct_val.char_val = 'g';
  struct_val.double_val = 6.28;

  void * data2 = talloc(struct_val);
  byte_pack(data2,struct_val);

  pod_struct recovered_struct;
  byte_unpack(data2,recovered_struct);
  free(data2);

//  result += strcmp(char_string,recovered_string);
  result += test("recover integer",int_val,recovered_int);
  result += test("recover double",double_val,recovered_double);
  result += test("recover struct int",struct_val.int_val,recovered_struct.int_val);
  result += test("recover struct char",struct_val.char_val,recovered_struct.char_val);
  result += test("recover struct double",struct_val.double_val,recovered_struct.double_val);

  std::cout << "Errors: " << result << std::endl;

//  MPI_Finalize();
  return result;
}
