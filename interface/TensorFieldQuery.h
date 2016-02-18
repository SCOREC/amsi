#ifndef TENSORFIELDQUERY_H_
#define TENSORFIELDQUERY_H_
#include <stdarg.h>
#include <string.h>
namespace amsi
{
  class TensorFieldQuery
  {
  public:
    virtual ~TensorFieldQuery() {};
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
#endif
