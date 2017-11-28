#ifndef APF_MESH_ITERATOR_H_
#define APF_MESH_ITERATOR_H_
#include <apf.h>
#include <apfMesh.h>
namespace amsi
{
  class apfMeshIterator
  {
  public:
    virtual void operator++() = 0;
    virtual bool operator==(const apfMeshIterator &) const = 0;
    virtual bool operator!=(const apfMeshIterator & o) const {return !operator==(o);}
    virtual apf::MeshEntity * operator*() const = 0;
  };
  class FullMeshIterator : public apfMeshIterator
  {
  private:
    apf::Mesh * msh;
    apf::MeshIterator * itr;
    apf::MeshEntity * crt;
    int ent_dim;
    FullMeshIterator();
  protected:
    static FullMeshIterator makeEnd(FullMeshIterator & o)
    {
      FullMeshIterator end(o.msh,o.ent_dim);
      end.crt = 0;
      return end; // copy elision should move this
    }
    FullMeshIterator(apf::Mesh * m, int ed)
      : msh(m)
      , itr(NULL)
      , crt(NULL)
      , ent_dim(ed)
    {
      itr = msh->begin(ed);
      this->operator++();
    }
    friend FullMeshIterator begin(apf::Mesh *,int);
    friend FullMeshIterator end(FullMeshIterator &);
  public:
    ~FullMeshIterator()
    {
      msh->end(itr);
    }
    virtual void operator++()
    {
      crt = msh->iterate(itr);
    }
    virtual bool operator==(const apfMeshIterator & o) const
    {
      const FullMeshIterator & ot = static_cast<const FullMeshIterator&>(o);
      bool rslt = true;
      rslt = rslt ? ot.crt == crt : false;
      rslt = rslt ? ot.msh == msh : false;
      rslt = rslt ? ot.ent_dim == ent_dim : false;
      return rslt;
    }
    virtual apf::MeshEntity * operator*() const
    {
      return crt;
    }
  };
  FullMeshIterator begin(apf::Mesh * msh, int ent_dim);
  FullMeshIterator end(FullMeshIterator & itr);
}
#endif
