// in-build
#include "../../test.h"
// in-project
#include "amsiProcessSet.h"
// standard
#include <iostream>
#include <utility>
int main(int, char**)
{
  using namespace amsi;
  int failed = 0;
  // test range set
  ProcessSet * p_zero_set = static_cast<ProcessSet*>(new ProcessSet_T<std::pair<int,int> >(std::make_pair(-1,-1)));
  failed += test_neq("ProcessSet(0)",static_cast<ProcessSet*>(NULL),p_zero_set);
  failed += test(".operator[]",-1,p_zero_set->operator[](20));
  failed += test(".size",0,p_zero_set->size());
  failed += test(".isIn(-1)",false,p_zero_set->isIn(-1));
  failed += test(".isIn(0)",false,p_zero_set->isIn(0));
  ProcessSet * p_macro_set = static_cast<ProcessSet*>(new ProcessSet_T<std::pair<int,int> >(std::make_pair(0,8)));
  ProcessSet * p_micro_set = static_cast<ProcessSet*>(new ProcessSet_T<std::pair<int,int> >(std::make_pair(8,24)));
  failed += test_neq("ProcessSet(0,8)",static_cast<ProcessSet*>(NULL),p_macro_set);
  failed += test_neq("ProcessSet(8,24)",static_cast<ProcessSet*>(NULL),p_micro_set);
  failed += test(".operator[]",5,p_macro_set->operator[](5));
  failed += test(".size()",16,p_micro_set->size());
  failed += test(".isIn()",true,p_micro_set->isIn(8));
  failed += test(".isIn()",false,p_micro_set->isIn(24));
  failed += test(".indexOf()", -1, p_micro_set->indexOf(0));
  failed += test(".indexOf()", 0, p_micro_set->indexOf(8));
  failed += test(".indexOf()", -1, p_micro_set->indexOf(24));
  failed += test(".indexOf()", 3, p_micro_set->indexOf(11));
  // test extracted sets
  ProcessSet * p_full_set = static_cast<ProcessSet*>(new ProcessSet_T<std::pair<int,int> >(std::make_pair(0,24)));
  ProcessSet * p_macro_extracted = p_full_set->extract(8);
  ProcessSet * p_micro_extracted = p_full_set->extract(16);
  failed += test_neq("Extracted ProcessSet(0,8)",static_cast<ProcessSet*>(NULL),p_macro_extracted);
  failed += test_neq("Extracted ProcessSet(8,24)",static_cast<ProcessSet*>(NULL),p_micro_extracted);
  failed += test(".operator[]",5,p_macro_extracted->operator[](5));
  failed += test(".size()",8,p_macro_extracted->size());
  failed += test(".size()",16,p_micro_extracted->size());
  failed += test(".isIn()",true,p_micro_extracted->isIn(8));
  failed += test(".isIn()",false,p_micro_extracted->isIn(24));
  failed += test(".indexOf()", -1, p_micro_extracted->indexOf(0));
  failed += test(".indexOf()", 0, p_micro_extracted->indexOf(8));
  failed += test(".indexOf()", -1, p_micro_extracted->indexOf(24));
  failed += test(".indexOf()", 3, p_micro_extracted->indexOf(11));
  // test set set
  ProcessSet * p_zero_set2 = static_cast<ProcessSet*>(new ProcessSet_T<std::set<int> >(-1,-1));
  failed += test_neq("ProcessSet(0)",static_cast<ProcessSet*>(NULL),p_zero_set2);
  failed += test(".operator[]",-1,p_zero_set2->operator[](20));
  failed += test(".size",0,p_zero_set2->size());
  failed += test(".isIn(-1)",false,p_zero_set2->isIn(-1));
  failed += test(".isIn(0)",false,p_zero_set2->isIn(0));
  ProcessSet * p_macro_set2 = static_cast<ProcessSet*>(new ProcessSet_T<std::set<int> >(0,8));
  ProcessSet * p_micro_set2 = static_cast<ProcessSet*>(new ProcessSet_T<std::set<int> >(8,24));
  failed += test_neq("ProcessSet(0,8)",static_cast<ProcessSet*>(NULL),p_macro_set2);
  failed += test(".operator[]",5,p_macro_set2->operator[](5));
  failed += test(".size()",8,p_macro_set2->size());
  failed += test(".size()",16,p_micro_set2->size());
  failed += test(".isIn()", true, p_micro_set2->isIn(8));
  failed += test(".isIn()", false, p_micro_set2->isIn(24));
  failed += test(".indexOf()", -1, p_micro_set->indexOf(0));
  failed += test(".indexOf()", 0, p_micro_set->indexOf(8));
  failed += test(".indexOf()", -1, p_micro_set->indexOf(24));
  failed += test(".indexOf()", 3, p_micro_set->indexOf(11));
  // test extracted sets
  ProcessSet* p_full_set2 =
      static_cast<ProcessSet*>(new ProcessSet_T<std::set<int> >(0, 24));
  ProcessSet* p_macro_extracted2 = p_full_set2->extract(8);
  ProcessSet* p_micro_extracted2 = p_full_set2->extract(16);
  /*
  failed += test_neq("Extracted
  ProcessSet(0,8)",static_cast<ProcessSet*>(NULL),p_macro_extracted2); failed +=
  test_neq("Extracted
  ProcessSet(8,24)",static_cast<ProcessSet*>(NULL),p_micro_extracted2); failed
  += test(".operator[]",5,p_macro_extracted2->operator[](5)); failed +=
  test(".size()",8,p_macro_extracted2->size()); failed +=
  test(".size()",16,p_micro_extracted2->size()); failed +=
  test(".isIn()",true,p_micro_extracted2->isIn(8)); failed +=
  test(".isIn()",false,p_micro_extracted2->isIn(24)); failed +=
  test(".indexOf()", -1, p_micro_extracted2->indexOf(0)); failed +=
  test(".indexOf()", 0, p_micro_extracted2->indexOf(8)); failed +=
  test(".indexOf()", -1, p_micro_extracted2->indexOf(24)); failed +=
  test(".indexOf()", 3, p_micro_extracted2->indexOf(11));
  // Test strided extraction
  delete p_full_set2;
  delete p_macro_extracted2;
  delete p_micro_extracted2;
  p_full_set2 = static_cast<ProcessSet*>(new ProcessSet_T<std::set<int>
  >(0,24)); p_macro_extracted2 =
  static_cast<ProcessSet*>(static_cast<ProcessSet_T<std::set<int>>*>(p_full_set2)->extractStrided(8,3,true));
  p_micro_extracted2 =
  static_cast<ProcessSet*>(static_cast<ProcessSet_T<std::set<int>>*>(p_full_set2)->extractStrided(16,3,false));
  failed += test_neq("Extracted
  ProcessSet(0,8)",static_cast<ProcessSet*>(NULL),p_macro_extracted2); failed +=
  test_neq("Extracted
  ProcessSet(8,24)",static_cast<ProcessSet*>(NULL),p_micro_extracted2); int size
  = p_macro_extracted2->size(); int * arr = (int*)malloc(sizeof(int)*size);
  toArray(p_macro_extracted2, arr);
  std::cout<<"Macro Array\n";
  for(int i=0; i<size; ++i)
  {
    std::cout<<arr[i]<<" ";
  }
  std::cout<<"\n";
  size = p_micro_extracted2->size();
  int * arr2 = (int*)malloc(sizeof(int)*size);
  std::cout<<"Micro Array\n";
  toArray(p_micro_extracted2, arr2);
  for(int i=0; i<size; ++i)
  {
    std::cout<<arr2[i]<<" ";
  }
  std::cout<<"\n";
  failed += test(".operator[]",15,p_macro_extracted2->operator[](5));
  failed += test(".size()",8,p_macro_extracted2->size());
  failed += test(".size()",16,p_micro_extracted2->size());
  failed += test(".isIn()",true,p_macro_extracted2->isIn(21));
  failed += test(".isIn()",false,p_micro_extracted2->isIn(21));
  failed += test(".isIn()",false,p_macro_extracted2->isIn(8));
  failed += test(".isIn()",true,p_micro_extracted2->isIn(8));
  failed += test(".isIn()",false,p_micro_extracted2->isIn(24));
  failed += test(".indexOf()", -1, p_micro_extracted2->indexOf(0));
  failed += test(".indexOf()", 5, p_micro_extracted2->indexOf(8));
  failed += test(".indexOf()", -1, p_micro_extracted2->indexOf(24));
  failed += test(".indexOf()", 7, p_micro_extracted2->indexOf(11));
  failed += test(".indexOf()", 0, p_micro_extracted2->indexOf(1));
  failed += test(".indexOf()", 7, p_macro_extracted2->indexOf(21));
  failed += test(".operator[]", 21, p_macro_extracted2->operator[](7));
  failed += test(".indexOf()", 0, p_macro_extracted2->indexOf(0));
   */
  return failed;
}
