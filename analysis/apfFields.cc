#include "apfFields.h"
#include "apfFunctions.h"
#include "amsiFields.h"
#include <apf.h>
#include <cassert>
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
  apf::Field * buildXpYField(const char * nm, apf::Field * x, apf::Field * y)
  {
    int vt = apf::getValueType(x);
    assert(vt == apf::getValueType(y));
    apf::FieldShape * xfs = apf::getShape(x);
    // field shapes are statically allocated objects, so if two fields have
    // the same fieldshape they should point to the same object, this is an
    // assumption using underlying knowledge that might change at some point,
    // so a more robust solution (such as comparing fieldshape names) would be better
    assert(xfs == apf::getShape(y)); 
    // this is a memory leak when the field is destroyed
    XpYFunc * fnc = new XpYFunc(x,y); 
    return apf::createUserField(apf::getMesh(x),nm,vt,xfs,fnc);
  }
}
