// in-build
#include "../../test.h"

// in-project
#include "ProcessSet.h"

// standard
#include <iostream>
#include <utility>

int main(int, char**)
{
  using namespace amsi;

  // Create a couple of ProcessSets, one with ranks [0,7] and the other with ranks [8,23]
  ProcessSet * p_macro_set = static_cast<ProcessSet*>(new ProcessSet_T<std::pair<int,int> >(std::make_pair(0,8)));
  ProcessSet * p_micro_set = static_cast<ProcessSet*>(new ProcessSet_T<std::pair<int,int> >(std::make_pair(8,24)));

  int failed = 0;
  
  // Various tests
  failed += test(".operator[]",5,p_macro_set->operator[](5));
  failed += test(".size()",16,p_micro_set->size());
  failed += test(".isIn()",true,p_micro_set->isIn(8));
  failed += test(".isIn()",false,p_micro_set->isIn(24));

  return failed;
}
