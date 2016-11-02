#ifndef AMSI_FIELDS_H_
#define AMSI_FIELDS_H_
#include "amsiLAS.h"
#include "sim.h"
#include <apf.h>
#include <vector>
namespace amsi
{
  // note: modifications here MUST be reflected in amsiFields.cc::fld_strs
  enum FieldType
  {
    UNTYPED = 0,
    DISPLACEMENT = 1,
    NUM_FIELD_TYPES = 2
  };
  char const * fieldTypeString(int tp);
  int decodeFieldType(const std::string & fld_nm);
  template <class O>
    void buildFieldsFromSim(pGModel mdl,
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
