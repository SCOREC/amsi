#include "apfFields.h"
#include "amsiFields.h"
#include "apf.h"
#include <pystring.h>
namespace amsi
{
  apf::Field * buildFieldFromSpec(apf::Mesh * msh,
                                  FieldSpec * fs)
  {
    std::string nm = composeFieldName(fs->nt,fs->tp,fs->nm);
    return apf::createLagrangeField(msh,
                                    nm.c_str(),
                                    fs->vt,
                                    fs->rd);
  }
}
