#include "amsiFields.h"
#include <cassert>
#include <pystring.h>
namespace amsi
{
  static char const * fld_units[] =
  {
    "untyped",
    "displacement"
  };
  static char const * fld_tps[] =
  {
    "full",
    "delta"
  };
  char const * fieldUnitString(int nt)
  {
    assert(nt >= 0 && nt < NUM_FIELD_UNITS);
    return fld_units[nt];
  }
  char const * fieldTypeString(int tp)
  {
    assert(tp >= 0 && tp < NUM_FIELD_TYPES);
    return fld_tps[tp];
  }
  int decodeFieldUnit(const std::string & fld_nm)
  {
    std::vector<std::string> tks;
    pystring::split(fld_nm,tks,std::string("_"));
    int tp = std::distance(&fld_units[0],std::find(&fld_units[0],&fld_units[NUM_FIELD_UNITS],tks[0]));
    assert(tp < NUM_FIELD_UNITS);
    return tp;
  }
  int decodeFieldType(const std::string & fld_nm)
  {
    std::vector<std::string> tks;
    pystring::split(fld_nm,tks,std::string("_"));
    int tp = std::distance(&fld_tps[0],std::find(&fld_tps[0],&fld_tps[NUM_FIELD_TYPES],tks[2]));
    assert(tp < NUM_FIELD_TYPES);
    return tp;
  }
  std::string decodeFieldName(apf::Field * fld)
  {
    std::vector<std::string> tks;
    pystring::split(apf::getName(fld),tks,std::string("_"));
    assert(tks.size() == 5);
    return tks[4];
  }
}
