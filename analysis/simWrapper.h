#ifndef SIMWRAPPER_H_
#define SIMWRAPPER_H_
#include "amsiHost.h"
#include <SimAttribute.h>
#include <iterator>
namespace amsi
{
  // creating multiple end iterators on the same list will result in extra NULL entries in the list, should remove them when destroying the iterator?
  template <typename T>
    class ListIterator : public std::iterator<std::input_iterator_tag, T>
  {
  private:
    void * ent;
    union {
      int v[2];
      void * it;
    } tmp; // assumes sizeof(void*) == 2*sizeof(int)
    pPList lst;
  public:
    ListIterator(pPList l, int n = 0)
      : ent(NULL)
      , tmp()
      , lst(l)
    {
      tmp.it = NULL;
      operator++();
      int sz = PList_size(lst);
      if(n > 0)
      {
        if(sz == n)
        {
          PList_append(lst,NULL);
          sz++;
        }
        tmp.v[0] = sz;
        ent = PList_item(lst,n);
      }
    }
    T operator*()
    {
      return static_cast<T>(ent);
    }
    ListIterator & operator++()
    {
      ent = PList_next(lst,&tmp.it);
      return *this;
    }
    bool operator==(ListIterator o)
    {
      return (o.lst == lst) && (o.tmp.it == tmp.it) && (o.ent == ent);
    }
    bool operator!=(ListIterator o)
    {
      return ! this->operator==(o);
    }
  };
  template <typename T>
    ListIterator<T> begin(pPList lst)
  {
    return ListIterator<T>(lst);
  }
  template <typename T>
    ListIterator<T> nth(pPList lst, int n)
  {
    return ListIterator<T>(lst,n);
  }
  template <typename T>
    ListIterator<T> end(pPList lst)
  {
    int sz = PList_size(lst);
    return ListIterator<T>(lst,sz);
  }
  template <typename T, typename O>
    void cutPaste(pPList lst, O out)
  {
    std::copy(begin<T>(lst),end<T>(lst),out);
    PList_delete(lst);
  }
  const char * modelItemTypeString(int tp);
  int modelItemTypeDim(int tp);
}
#endif
