#include "apfMeshIterator.h"
namespace amsi
{
  FullMeshIterator begin(apf::Mesh * msh, int ent_dim)
  {
    return FullMeshIterator(msh,ent_dim);
  }
  FullMeshIterator end(FullMeshIterator & itr)
  {
    return FullMeshIterator::makeEnd(itr);
  }
}
