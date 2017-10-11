#ifndef SIM_CLASSIFIED_H_
#define SIM_CLASSIFIED_H_
#include "simIterator.h"
#include <apfSIM.h>
namespace amsi
{
  class SimClassifiedIter : public SimIterator
  {
  protected:
    apf::MeshSIM * msh;
    apf::ModelEntity * mdl_ent;
    int dim;
    union GIter
    {
      RIter riter;
      FIter fiter;
      EIter eiter;
      VIter viter;
    } it;
    union pGEnt
    {
      pRegion rgn;
      pFace   fac;
      pEdge   edg;
      pVertex vtx;
    } ent;
    void end();
    friend SimClassifiedIter endClassified(const SimClassifiedIter &);
  public:
    SimClassifiedIter(apf::Mesh * m, apf::ModelEntity * me, int d);
    bool operator==(const SimIterator & o) const;
    void operator++();
    apf::MeshEntity * operator*() const;
  };
  SimClassifiedIter beginClassified(apf::Mesh * msh, apf::ModelEntity * mdl_ent, int dim);
  SimClassifiedIter endClassified(const SimClassifiedIter & bgn);
}
#endif
