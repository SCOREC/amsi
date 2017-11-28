#include "simClassified.h"
#include "sim.h"
#include <apfSIM.h>
namespace amsi
{
  SimClassifiedIter::SimClassifiedIter(apf::Mesh * m, apf::ModelEntity * me, int d)
    : apfMeshIterator()
    , msh(reinterpret_cast<apf::MeshSIM*>(m))
    , mdl_ent(me)
    , dim(d)
    , it()
    , ent()
  {
    switch(dim)
    {
    case 0:
      it.viter = M_classifiedVertexIter(PM_mesh(msh->getMesh(),0),reinterpret_cast<pGEntity>(mdl_ent),1);
      ent.vtx = VIter_next(it.viter);
      break;
    case 1:
      it.eiter = M_classifiedEdgeIter(PM_mesh(msh->getMesh(),0),reinterpret_cast<pGEntity>(mdl_ent),1);
      ent.edg = EIter_next(it.eiter);
      break;
    case 2:
      it.fiter = M_classifiedFaceIter(PM_mesh(msh->getMesh(),0),reinterpret_cast<pGEntity>(mdl_ent),1);
      ent.fac = FIter_next(it.fiter);
      break;
    case 3:
      it.riter = M_classifiedRegionIter(PM_mesh(msh->getMesh(),0),reinterpret_cast<pGEntity>(mdl_ent));
      ent.rgn = RIter_next(it.riter);
      break;
    };
  }
  void SimClassifiedIter::end()
  {
    ent.vtx = NULL;
  }
  bool SimClassifiedIter::operator==(const apfMeshIterator & o) const
  {
    const SimClassifiedIter * ot = static_cast<const SimClassifiedIter*>(&o);
    if(ot->msh == msh && ot->mdl_ent == mdl_ent && ot->dim == dim)
    {
      switch(dim)
      {
      case 0:
        return ot->ent.vtx == ent.vtx;
      case 1:
        return ot->ent.edg == ent.edg;
      case 2:
        return ot->ent.fac == ent.fac;
      case 3:
        return ot->ent.rgn == ent.rgn;
      }
    }
    return false;
  }
  void SimClassifiedIter::operator++()
  {
    switch(dim)
    {
    case 0:
      ent.vtx = VIter_next(it.viter);
      break;
    case 1:
      ent.edg = EIter_next(it.eiter);
      break;
    case 2:
      ent.fac = FIter_next(it.fiter);
      break;
    case 3:
      ent.rgn = RIter_next(it.riter);
      break;
    }
  }
  apf::MeshEntity * SimClassifiedIter::operator*() const
  {
    switch(dim)
    {
    case 0:
      return apf::castEntity((pEntity)ent.vtx);
      break;
    case 1:
      return apf::castEntity((pEntity)ent.edg);
      break;
    case 2:
      return apf::castEntity((pEntity)ent.fac);
      break;
    case 3:
      return apf::castEntity((pEntity)ent.rgn);
      break;
    }
    return NULL;
  }
  SimClassifiedIter beginClassified(apf::Mesh * msh, apf::ModelEntity * mdl_ent, int dim)
  {
    return SimClassifiedIter(msh,mdl_ent,dim);
  }
  SimClassifiedIter endClassified(const SimClassifiedIter & bgn)
  {
    SimClassifiedIter it(bgn);
    it.end();
    return it;
  }
}
