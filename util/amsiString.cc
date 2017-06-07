#include "amsiString.h"
#include <iostream>
namespace amsi
{
  bool string2Bool(const std::string & str)
  {
    if(str == std::string("True") || str == std::string("true") || str == std::string("t") || str == std::string("T"))
      return true;
    if(str == std::string("False") || str == std::string("false") || str == std::string("f") || str == std::string("F"))
      return false;
    std::cerr << "ERROR: cannot convert string (" << str << ") to boolean" << std::endl;
    return false;
  }
}
