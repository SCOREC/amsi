#ifndef SIM_ITERATOR_H_
#define SIM_ITERATOR_H_
#include <apfMesh.h>

namespace amsi
{
  class SimIterator
  {
  public:
    virtual void operator++() = 0;
    virtual bool operator==(const SimIterator &) const = 0;
    virtual apf::MeshEntity * operator*() const = 0;
    virtual operator apf::MeshEntity*() = 0;
  };
}
#endif
