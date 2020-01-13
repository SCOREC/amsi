#ifndef SIM_CLASSIFIED_H_
#define SIM_CLASSIFIED_H_
#include <apfSIM.h>
namespace amsi
{
  class SimClassifiedIter
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
    void  init();
    friend SimClassifiedIter endClassified(const SimClassifiedIter &);
  public:
    SimClassifiedIter(apf::Mesh * m, apf::ModelEntity * me, int d);
    // construct iter can be set to false if we don't actually want to construct
    // the classified iterator which can take some time...this is useful in the
    // case of creating the end iterator
    SimClassifiedIter(const SimClassifiedIter & itr, bool construct_itr=true);
    ~SimClassifiedIter();
    bool operator==(const SimClassifiedIter & o) const;
    bool operator!=(const SimClassifiedIter & o) const { return !operator==(o); }
    void operator++();
    apf::MeshEntity * operator*() const;
  };
  SimClassifiedIter beginClassified(apf::Mesh * msh, apf::ModelEntity * mdl_ent, int dim);
  SimClassifiedIter endClassified(const SimClassifiedIter & bgn);
}
#endif
