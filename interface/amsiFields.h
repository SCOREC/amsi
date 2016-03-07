#ifndef AMSI_FIELDS_H_
#define AMSI_FIELDS_H_
#include "amsiLAS.h"
#include <vector>
namespace amsi
{
  enum Fields
  {
    DISPLACEMENT = 0,
    NUM_FIELDS = 1
  };
  char const * getFieldString(int tp);
//  template <class T>
//    void buildFieldsFromSim(pACase);
}
#endif
