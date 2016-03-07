#ifndef APF_FIELDS_H_
#define APF_FIELDS_H_
#include "amsiFields.h"
namespace amsi
{
  struct apfFieldSpec
  {
    apf::Mesh * msh;
    const char * nm;
    int vl_tp;
    int o;
  };
  apf::Field * buildFromSpec(FieldSpec * fs);
  class apfConstrainedField : public ConstrainedTensorField
  {
  protected:
    apf::Field * fld;
    apf::Numbering * nm;
  public:
    apfConstrainedField(apf::Field * fld);
    apf::Field * getField();
    apf::Numbering * getNumbering();
  };
}
#endif
