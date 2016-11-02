#include "amsiFields.h"
#include <cassert>
#include <pystring.h>
namespace amsi
{
  static char const * fld_strs[] =
  {
    "untyped"
    "displacement"
  };
  char const * fieldTypeString(int tp)
  {
    assert(tp >= 0 && tp < NUM_FIELD_TYPES);
    return fld_strs[tp];
  }
  int decodeFieldType(const std::string & fld_nm)
  {
    std::vector<std::string> tks;
    pystring::partition(fld_nm,std::string("_"),tks);
    int tp = std::distance(&fld_strs[0],std::find(&fld_strs[0],&fld_strs[NUM_FIELD_TYPES],tks[0]));
    assert(tp < NUM_FIELD_TYPES);
    return tp;
  }
}
