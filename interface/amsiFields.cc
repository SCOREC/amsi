#include "amsiFields.h"
namespace amsi
{
  char const * getFieldString(int tp)
  {
    static char const * fld_strs[] =
    {
      "displacement"
    };
    return fld_strs[tp];
  }
}
