#include "apfField.h"
#include "apfShape.h"
#include "apfNumbering.h"
#include <cassert>
namespace amsi
{
  template <typename I>
    int applyDirichletBC(apf::Numbering * nm,
                         I begin,
                         I end,
                         BCQuery * qry,
                         double t)
  {
    int fxd = 0;
    apf::Field * fld = apf::getField(nm);
    apf::Mesh * msh = apf::getMesh(fld);
    apf::FieldShape * fs = apf::getShape(fld);
    int cmps = apf::countComponents(fld);
    assert(qry->numComps() == cmps);
    for(I it = begin; it != end; ++it)
    {
      // TODO : would prefer to implement a wrapper on the sim iterator...
      apf::MeshEntity * ent = reinterpret_cast<apf::MeshEntity*>(*it);
      int nds = fs->countNodesOn(msh->getType(ent));
      for(int ii = 0; ii < nds; ii++)
      {
        double vls[cmps] = {};
        apf::getComponents(fld,ent,ii,vls);
        for(int jj = 0; jj < cmps; jj++)
          if(qry->isFixed(jj))
          {
            apf::fix(nm,ent,ii,jj,true);
            fxd++;
            vls[jj] = getDirichletValue(qry,msh,ent,ii,jj,t);
          }
        apf::setComponents(fld,ent,ii,vls);
      }
    }
    return fxd;
  }
  template <typename I>
    void applyNeumannBC(LAS * las,
                        apf::Numbering * nm,
                        I bgn,
                        I nd,
                        NeumannIntegrator * i,
                        double t)
  {
    apf::Field * fld = apf::getField(nm);
    for(I it = bgn; it != nd; ++it)
    {
      apf::MeshEntity * ent = reinterpret_cast<apf::MeshEntity*>(*it);
      i->process(apf::createMeshElement(apf::getMesh(fld),ent));
      apf::NewArray<int> dofs;
      apf::getElementNumbers(nm,ent,dofs);
      las->AddToVector(i->getnedofs(),&dofs[0],&i->getFe()[0]);
    }
  }
}
