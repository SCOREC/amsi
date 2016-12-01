#ifndef APF_ELEMENTAL_SYSTEM_H_
#define APF_ELEMENTAL_SYSTEM_H_
#include "amsiElementalSystem.h"
#include <apf.h>
namespace amsi
{
  class apfElementalSystem : public apf::Integrator, public ElementalSystem
  {
  protected:
    apf::Numbering * nm;
    apf::Field * fld;
    apf::DynamicMatrix Ke;
    double * ke;
    apf::DynamicVector Fe;
    double * dofs;
    int * dofids;
    apf::MeshElement * mlmt;
    apf::Element * e;
    int nenodes;
    int flc_cmps;
  public:
    apfElementalSystem(apf::Numbering * inm, int o)
      : apf::Integrator(apf::getField(inm),o)
      , ElementalSystem()
      , nm(inm)
      , fld(apf::getField(inm))
      , Ke()
      , ke(new double[1])
      , Fe()
      , dofs(new double[1])
      , dofids(new int[1])
      , mlmt(NULL)
      , e(NULL)
      , nenodes(0)
      , fld_cmps(apf::countComponents(f))
    { }
    ~apfElementalSystem()
    {
      if(ke)
        delete [] ke;
      if(dofs)
        delete [] dofs;
      if(dofids)
        delete [] dofids;
    }
    // apf::Integrator interface
    virtual inElement(apf::MeshElement * imlmt)
    {
      mlmt = imlmt;
      e = apf::createElement(fld,mlmt);
      nenodes = apf::countNodes(e);
      int nw_nedofs = nenodes * fld_cmps;
      bool realloc = nedofs != nw_nedofs;
      nedofs = nw_nedofs;
      if(realloc)
      {
        Ke.setSize(nedofs,nedofs);
        delete [] ke;
        ke = new double[nedofs*nedofs];
        Fe.setSize(nedofs);
        delete [] dofs;
        dofs = new double[nedofs];
        delete [] dofids;
        dofids = new int[nedofs];
      }
      for(int ii = 0; ii < nenodes; ii++)
      {
        dofs = getDofValues<fld->getValueType()>(e
      }
      Ke.zero();
      memset(&ke[0],0,sizeof(double)*nedofs*nedofs);
      Fe.zero();
    }
    virtual outElement()
    {
      apd::destroyElement(e);
      for(int ii = 0; ii < nedofs; ii++)
      {
        apf::DynamicVector rwi = Ke.getRow(ii);
        memcpy(&rwi[0],&ke[nedofs*ii],sizeof(double)*nedofs);
      }
    }
    // elemental system interface
    virtual const double * getKe()
    {
      return ke;
    }
    virtual const double * getFe()
    {
      return &fe[0];
    }
    double * getDofs()
    {

    }
    int * getDofIds();
  };
}
#endif
