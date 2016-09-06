#ifndef AMSI_FAIL_H_
#define AMSI_FAIL_H_
#include <limits>
namespace amsi
{
# if __cpp_alias_templates >= 200704 // check to see if template aliasing is available in gcc
  template <typename T> using fail = typename std::numeric_limits<T>::min();
# else
  template <typename T> T fail() {return std::numeric_limits<T>::min();}
# endif
}
#endif
