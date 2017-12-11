#ifndef APF_FIELDS_H_
#define APF_FIELDS_H_
#include "apf.h"
namespace amsi
{
  struct FieldSpec;
  apf::Field * buildFieldFromSpec(apf::Mesh * msh, FieldSpec * fs);
  apf::Field * buildXpYField(const char * nm, apf::Field * x, apf::Field * y);
}
#endif
