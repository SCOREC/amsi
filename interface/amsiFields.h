#ifndef AMSI_FIELDS_H_
#define AMSI_FIELDS_H_
#include "amsiLAS.h"
#include "sim.h"
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
    int unit;
    int type;
    std::string name;
  };
  char const * fieldUnitString(int nt);
  char const * fieldTypeString(int tp);
  int decodeFieldUnit(const std::string & fld_nm);
  int decodeFieldType(const std::string & fld_nm);
  std::string decodeFieldName(apf::Field * fld);
  template <class O>
    void buildFieldsFromSim(pACase cs,
                            apf::Mesh * msh,
                            O out);
  template <class I, class O>
    void buildNumberingsFromSim(pGModel mdl,
                                I fld_bgn,
                                I fld_nd,
                                O out);
}
#include "amsiFields_impl.h"
#endif
