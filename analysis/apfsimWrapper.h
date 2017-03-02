#ifndef APF_SIM_WRAPPER_H_
#define APF_SIM_WRAPPER_H_
#include "sim.h"
#include <apf.h>
#include <apfMesh.h>
#include <list>
namespace amsi
{
  double measureEntity(pGEntity ent, pMesh msh, apf::Mesh * apf_mesh);
  double measureEntityFromSurf(pGEntity ent, pMesh msh, apf::Mesh * apf_mesh);
   /*
  template <int dim>
    class SimIterator
  {
  private:
    pMesh msh;
  public:
    pEntity operator*();
    SimIterator<dim> & operator++();
    bool operator(SimIterator<dim> & o);
  };
  class sim2apfMeshIterator
  {
  protected:
    apf::Mesh * msh;
    pGEntity mdl_ent;
    int dm;
  public:
    MeshIterator(apf::Mesh * imsh, pGEntity me, int d)
      : msh(imsh)
      , mdl_ent(me)
      , dm(d)
    { }
    apf::MeshEntity * operator*()
    {
      return apf::castEntity(
    }
    MeshIterator & operator++()
    {
    }
  };
  class ClassifiedIterator
  {
  private:
    apf::Mesh * msh;
    apf::MeshEntity * ent;
    pGEntity mdl_ent;
    int dm;
  public:
    ClassifiedIterator & operator++()
    {

    }
  };
  */
  void applyUniqueRegionTags(pGModel mdl, pMesh msh, apf::Mesh * apfmsh);
}
#endif
