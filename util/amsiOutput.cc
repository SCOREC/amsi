#include "amsiOutput.h"
namespace amsi
{
  void suppressOutput(std::ostream & io)
  {
    // redirect std::cout to /dev/null in silent processes.
    // update: apparently setting the failbit will suppress all cout usage, without needing a /dev/null stream
    io.setstate(std::ios_base::failbit);
  }
  void expressOutput(std::ostream & io)
  {
    io.setstate(std::ios_base::goodbit);
  }
}
