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

  int failed = 0;

  ProcessSet * p_zero_set = static_cast<ProcessSet*>(new ProcessSet_T<std::pair<int,int> >(std::make_pair(-1,-1)));

  failed += test_neq("ProcessSet(0)",static_cast<ProcessSet*>(NULL),p_zero_set);
  failed += test(".operator[]",-1,p_zero_set->operator[](20));
  failed += test(".size",0,p_zero_set->size());
  failed += test(".isIn(-1)",false,p_zero_set->isIn(-1));
  failed += test(".isIn(0)",false,p_zero_set->isIn(0));
  

  ProcessSet * p_macro_set = static_cast<ProcessSet*>(new ProcessSet_T<std::pair<int,int> >(std::make_pair(0,8)));
  ProcessSet * p_micro_set = static_cast<ProcessSet*>(new ProcessSet_T<std::pair<int,int> >(std::make_pair(8,24)));
  
  failed += test_neq("ProcessSet(0,8)",static_cast<ProcessSet*>(NULL),p_macro_set);
  failed += test(".operator[]",5,p_macro_set->operator[](5));
  failed += test(".size()",16,p_micro_set->size());
  failed += test(".isIn()",true,p_micro_set->isIn(8));
  failed += test(".isIn()",false,p_micro_set->isIn(24));

  return failed;
}
