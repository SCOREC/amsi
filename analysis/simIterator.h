#ifndef SIM_ITERATOR_H_
#define SIM_ITERATOR_H_
#include <apfMesh.h>
#include <memory>
namespace amsi
{
  class SimIterator
  {
  public:
    virtual void operator++() {};
    virtual bool operator==(const SimIterator &) const {return false;}
    virtual bool operator!=(const SimIterator & o) const {return !operator==(o);}
    virtual apf::MeshEntity * operator*() const {return NULL;}
  };
}
#endif
