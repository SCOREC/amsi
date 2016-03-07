#ifndef AMSI_APF_ANALYSIS_H_
#define AMSI_APF_ANALYSIS_H_
#include "apf.h"
namespace amsi
{
  template <typename I>
    void applyFEIntegrator(LAS * las,
                           apf::Numbering * nm,
                           I bgn,
                           I nd,
                           FEIntegrator * i,
                           double t)
  {
    apf::Field * fld = apf::getField(nm);
    for(I it = bgn; it != nd; ++it)
    {
      apf::MeshEntity * ent = reinterpret_case<apf::MeshEntity*>(*it);
      apf::MeshElement * mnt = apf::createMeshElement(apf::getMesh(fld),ent);
      i->process(mnt);
      apf::NewArray<int> dofs;
      apf::getElementNumbers(nm,ent,dofs);
      assemble(las,dofs.size(),&dofs[0],&i->getVals()[0],&i->getKe()[0],&i->getFe()[0],false);
      apf::destroyMeshElement(mnt);
    }
  }
}
#endif
