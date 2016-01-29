/******************************************************************************

  (c) 2014 Scientific Computation Research Center,
      Rensselaer Polytechnic Institute. All rights reserved.

  The LICENSE-SCOREC file included with this distribution describes the terms
  of the SCOREC Non-Commercial License this program is distributed under.

*******************************************************************************/

#ifndef TENSORFIELDQUERY_H_
#define TENSORFIELDQUERY_H_

// 
#include <stdarg.h>
#include <string.h>

namespace amsi {
  namespace Analysis {

    class TensorFieldQuery
    {
    public:
      virtual double* operator()(int,...) = 0;
      virtual void bind(int,double) = 0;
      virtual int bound() = 0;
      virtual int unbound() = 0;
      virtual void clear() = 0;
    };

    template <typename T>
      class TensorFieldQueryT : public TensorFieldQuery
    {
    public:
      virtual double* operator()(int,...) { return NULL; }
      virtual void bind(int,double) {};
      virtual int bound() { return 0; }
      virtual int unbound() { return 0; }
      virtual void clear() {};
    };

  }
}


#endif
