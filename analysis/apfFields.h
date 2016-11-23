#ifndef APF_FIELDS_H_
#define APF_FIELDS_H_
#include "apfField.h"
namespace amsi
{
  struct FieldSpec;
  apf::Field * buildFieldFromSpec(apf::Mesh * msh, FieldSpec * fs);
}
#endif
