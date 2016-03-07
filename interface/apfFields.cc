#include "apfFields.h"
namespace amsi
{
  apf::Field * buildFromSpec(FieldSpec * fs)
  {
    return apf::createLagrangeField(fs->msh,fs->nm,fs->vl_tp,fs->o);
  }
}
