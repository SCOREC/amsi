#ifndef AMSI_OUTPUT_H_
#define AMSI_OUTPUT_H_
#include <ostream>
#include "amsiMPI.h"
namespace amsi
{
  void suppressOutput(std::ostream & io);
  void expressOutput(std::ostream & io);
}
#endif
