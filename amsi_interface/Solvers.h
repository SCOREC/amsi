#ifndef FEASOLVERS_H_
#define FEASOLVERS_H_

#include "FEA.h"
#include "LAS.h"

#include <string>

namespace amsi {
  namespace Analysis {
    
    void LinearSolver(FEA*,LAS*);
    void NewtonSolver(FEA*,LAS*,int,double,double,double&);

  }
}
#endif
