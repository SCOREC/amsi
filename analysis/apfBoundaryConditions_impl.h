#ifndef APF_BOUNDARY_CONDITIONS_IMPL_H_
#define APF_BOUNDARY_CONDITIONS_IMPL_H_
#include "apfShape.h"
#include "apfNumbering.h"
#include "amsiNeumannIntegrators.h"
#include "amsiBoundaryConditions.h"
#include <cassert>
namespace amsi
{
  template <typename I>
    int applyDirichletBC(apf::Numbering * nm,
                         I begin,
                         I end,
                         BCQuery * qry,
                         double t,
                         apf::Field* deltaField)
  {
    int fxd = 0;
    apf::Field * fld = apf::getField(nm);
    apf::Mesh * msh = apf::getMesh(fld);
    apf::FieldShape * fs = apf::getShape(fld);
    int cmps = apf::countComponents(fld);
    double * vls = new double[cmps];
    double * old_vls = new double[cmps];
    assert(qry->numComps() == cmps);
    for(I it = begin; it != end; ++it)
    {
      // TODO : would prefer to implement a wrapper on the sim iterator...
      apf::MeshEntity * ent = reinterpret_cast<apf::MeshEntity*>(*it);
      int nds = fs->countNodesOn(msh->getType(ent));
      for(int ii = 0; ii < nds; ii++)
      {
        apf::getComponents(fld,ent,ii,vls);
        apf::getComponents(fld,ent,ii,old_vls);
        for(int jj = 0; jj < cmps; jj++)
        {
          if(qry->isFixed(jj))
          {
            apf::fix(nm,ent,ii,jj,true);
            fxd++;
            vls[jj] = getDirichletValue(qry,msh,ent,ii,jj,t);
          }
        }
        // if the user supplied a delta field such as the delta_U field
        // in a solid mechanics problem set this value as well
        // we should only really set the components that lie of the fixe surfaces
        if(deltaField)
        {
          for(int i=0; i<cmps; ++i)
            old_vls[i] = vls[i]-old_vls[i];
          apf::setComponents(deltaField,ent,ii,old_vls);
        }
        apf::setComponents(fld,ent,ii,vls);
      }
    }
    delete [] vls;
    delete [] old_vls;
    return fxd;
  }
  template <typename I>
    void applyNeumannBC(LAS * las,
                        apf::Numbering * nm,
                        I bgn,
                        I nd,
                        NeumannIntegrator * i,
                        double)
  {
    apf::Field * fld = apf::getField(nm);
    for(I it = bgn; it != nd; ++it)
    {
      apf::MeshEntity * ent = reinterpret_cast<apf::MeshEntity*>(*it);
      apf::MeshElement * mnt = apf::createMeshElement(apf::getMesh(fld),ent);
      i->process(mnt);
      apf::NewArray<int> dofs;
      apf::getElementNumbers(nm,ent,dofs);
      las->AddToVector(i->getnedofs(),&dofs[0],&i->getFe()[0]);
      apf::destroyMeshElement(mnt);
    }
  }
}
#endif
