#ifndef APF_WRAPPER_H_
#define APF_WRAPPER_H_
#include <apf.h>
namespace amsi
{
  void getDofValues(apf::Field * fld, apf::Element * elmt, double * vls);
};
#endif
