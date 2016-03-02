#ifndef APF_WRAPPER_H_
#define APF_WRAPPER_H_
#include <apf.h>
namespace amsi
{
  void getDofValues(apf::Field * fld, apf::Element * elmt, double * vls);
  // note : collective on AMSI_COMM_SCALE
  template <class I>
    void getAvgFieldValue(apf::Field * fld, I bgn, I nd, double * avg);
  class TagIterator
  {
  private:
  public:
    bool operator++();
    bool operator==(const TagIterator & o);
  };
};
#include "apfWrapper_impl.h"
#endif
