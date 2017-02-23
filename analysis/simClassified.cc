#include "simClassified.h"
#include "sim.h"
#include <apfSIM.h>
namespace amsi
{
  class SimClassifiedIter : public SimIterator
  {
  protected:
    apf::MeshSIM * msh;
    apf::ModelEntity * mdl_ent;
  public:
    SimClassifiedIter(apf::MeshSIM * m, apf::ModelEntity * me)
      : msh(m)
      , mdl_ent(me)
    { }
    ~SimClassifiedIter()
    { }
    bool operator==(const SimIterator & o) const
    {
      const SimClassifiedIter * ot = reinterpret_cast<const SimClassifiedIter*>(&o);
      return ot->msh == msh && ot->mdl_ent == mdl_ent;
    }
    operator apf::MeshEntity*()
    {
      return *(*this);
    }
  };
  class SimClassifiedRegionIter : public SimClassifiedIter
  {
  private:
    RIter it;
    pRegion pr;
  public:
    SimClassifiedRegionIter(apf::MeshSIM * m, apf::ModelEntity * me)
      : SimClassifiedIter(m,me)
    {
      it = M_classifiedRegionIter(PM_mesh(msh->getMesh(),0),
                                  reinterpret_cast<pGEntity>(mdl_ent));
      pr = RIter_next(it);
    }
    ~SimClassifiedRegionIter()
    {
      RIter_delete(it);
    }
    void operator++()
    {
      pr = RIter_next(it);
    }
    bool operator==(const SimIterator & o) const
    {
      if(SimClassifiedIter::operator==(o))
      {
        const SimClassifiedRegionIter * ot = reinterpret_cast<const SimClassifiedRegionIter*>(&o);
        return ot->pr == pr;
      }
      return false;
    }
    apf::MeshEntity * operator*() const
    {
      return apf::castEntity((pEntity)pr);
    }
  };
  class SimClassifiedFaceIter : public SimClassifiedIter
  {
  private:
    FIter it;
    pFace fc;
  public:
    SimClassifiedFaceIter(apf::MeshSIM * m, apf::ModelEntity * me)
      : SimClassifiedIter(m,me)
    {
      it = M_classifiedFaceIter(PM_mesh(msh->getMesh(),0),
                                reinterpret_cast<pGEntity>(mdl_ent),1);
      fc = FIter_next(it);
    }
    ~SimClassifiedFaceIter()
    {
      FIter_delete(it);
    }
    void operator++()
    {
      fc = FIter_next(it);
    }
    bool operator==(const SimIterator & o) const
    {
      if(SimClassifiedIter::operator==(o))
      {
        const SimClassifiedFaceIter * ot = reinterpret_cast<const SimClassifiedFaceIter*>(&o);
        return ot->fc == fc;
      }
      return false;
    }
    apf::MeshEntity* operator*() const
    {
      return apf::castEntity((pEntity)fc);
    }
  };
  class SimClassifiedEdgeIter : public SimClassifiedIter
  {
  private:
    EIter it;
    pEdge ed;
  public:
    SimClassifiedEdgeIter(apf::MeshSIM * m, apf::ModelEntity * me)
      : SimClassifiedIter(m,me)
    {
      it = M_classifiedEdgeIter(PM_mesh(msh->getMesh(),0),
                                reinterpret_cast<pGEntity>(mdl_ent),1);
      ed = EIter_next(it);
    }
    ~SimClassifiedEdgeIter()
    {
      EIter_delete(it);
    }
    void operator++()
    {
      ed = EIter_next(it);
    }
    bool operator==(const SimIterator & o) const
    {
      if(SimClassifiedIter::operator==(o))
      {
        const SimClassifiedEdgeIter * ot = reinterpret_cast<const SimClassifiedEdgeIter*>(&o);
        return ot->ed == ed;
      }
      return false;
    }
    apf::MeshEntity * operator*() const
    {
      return apf::castEntity((pEntity)ed);
    }
  };
  class SimClassifiedVertexIter : public SimClassifiedIter
  {
  private:
    VIter it;
    pVertex vt;
  public:
    SimClassifiedVertexIter(apf::MeshSIM * m, apf::ModelEntity * me)
      : SimClassifiedIter(m,me)
    {
      it = M_classifiedVertexIter(PM_mesh(msh->getMesh(),0),
                                  reinterpret_cast<pGEntity>(mdl_ent),1);
      vt = VIter_next(it);
    }
    ~SimClassifiedVertexIter()
    {
      VIter_delete(it);
    }
    void operator++()
    {
      vt = VIter_next(it);
    }
    bool operator==(const SimIterator & o) const
    {
      if(SimClassifiedIter::operator==(o))
      {
        const SimClassifiedVertexIter * ot = reinterpret_cast<const SimClassifiedVertexIter*>(&o);
        return ot->vt == vt;
      }
      return false;
    }
    apf::MeshEntity * operator*() const
    {
      return apf::castEntity((pEntity)vt);
    }
  };
  SimIterator * beginClassified(apf::MeshSIM * msh,
                                apf::ModelEntity * mdl_ent,
                                int dm)
  {
    switch(dm)
    {
    case 3:
      return new SimClassifiedRegionIter(msh,mdl_ent);
    case 2:
      return new SimClassifiedFaceIter(msh,mdl_ent);
    case 1:
      return new SimClassifiedEdgeIter(msh,mdl_ent);
    case 0:
      return new SimClassifiedVertexIter(msh,mdl_ent);
    }
    return NULL;
  }
  class EndSimIterator : public SimIterator
  {
  private:
    SimIterator * bgn;
  public:
    EndSimIterator(SimIterator * i)
      : bgn(i)
    { }
    void operator++()
    { }
    bool operator==(const SimIterator & o) const
    {
      if(bgn == &o && *o == NULL)
        return true;
      return false;
    }
    apf::MeshEntity * operator*() const
    {
      return NULL;
    }
  };
}
