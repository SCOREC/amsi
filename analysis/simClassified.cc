#include "simClassified.h"
#include "sim.h"
#include <apfSIM.h>
namespace amsi
{
  void SimClassifiedIter::init()
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
  SimClassifiedIter::SimClassifiedIter(apf::Mesh * m, apf::ModelEntity * me, int d)
    : msh(reinterpret_cast<apf::MeshSIM*>(m))
    , mdl_ent(me)
    , dim(d)
    , it()
    , ent()
  {
    init();
  }
  SimClassifiedIter::SimClassifiedIter(const SimClassifiedIter & itr, bool construct_itr)
  : msh(itr.msh)
  , mdl_ent(itr.mdl_ent)
  , dim(itr.dim)
  , it()
  , ent()
  {
    if(construct_itr)
    {
      init();
    }
    else
    {
      switch(dim)
      {
      case 0:
        it.viter = NULL;
        break;
      case 1:
        it.eiter = NULL;
        break;
      case 2:
        it.fiter = NULL;
        break;
      case 3:
        it.riter = NULL;
        break;
      }
    }
  }
  SimClassifiedIter::~SimClassifiedIter()
  {
    switch(dim)
    {
    case 0:
      VIter_delete(it.viter);
      break;
    case 1:
      EIter_delete(it.eiter);
      break;
    case 2:
      FIter_delete(it.fiter);
      break;
    case 3:
      RIter_delete(it.riter);
      break;
    };
  }
  void SimClassifiedIter::end()
  {
    switch(dim)
    {
    case 0:
      ent.vtx = NULL;
      break;
    case 1:
      ent.edg = NULL;
      break;
    case 2:
      ent.fac = NULL;
      break;
    case 3:
      ent.rgn = NULL;
      break;
    }
  }
  bool SimClassifiedIter::operator==(const SimClassifiedIter & ot) const
  {
    if(ot.msh == msh && ot.mdl_ent == mdl_ent && ot.dim == dim)
    {
      switch(dim)
      {
      case 0:
        return ot.ent.vtx == ent.vtx;
      case 1:
        return ot.ent.edg == ent.edg;
      case 2:
        return ot.ent.fac == ent.fac;
      case 3:
        return ot.ent.rgn == ent.rgn;
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
    SimClassifiedIter it(bgn, false);
    it.end();
    return it;
  }
}
