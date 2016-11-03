#ifndef AMSI_FIELDS_H_
#define AMSI_FIELDS_H_
#include "amsiLAS.h"
#include <apf.h>
#include <vector>
namespace amsi
{
  // note: modifications here MUST be reflected in amsiFields.cc::fld_strs
  enum FieldUnit
  {
    UNITLESS = 0,
    DISPLACEMENT = 1,
    NUM_FIELD_UNITS = 2
  };
  enum FieldType
  {
    FULL = 0,
    DELTA = 1,
    NUM_FIELD_TYPES = 2
  };
  struct FieldSpec
  {
    std::string nm;
    int nt;
    int tp;
    int vt;
    int rd;
  };
  char const * fieldUnitString(int nt);
  char const * fieldTypeString(int tp);
  int decodeFieldUnit(const std::string & fld_nm);
  int decodeFieldType(const std::string & fld_nm);
  std::string decodeFieldName(apf::Field * fld);
  std::string composeFieldName(int nt, int tp, const std::string & nm);
}
#include "amsiFields_impl.h"
#endif
