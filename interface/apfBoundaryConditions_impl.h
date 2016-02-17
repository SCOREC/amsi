#include "apfField.h"
#include "apfShape.h"
#include "apfNumbering.h"
#include <cassert>
namespace amsi
{
  template <typename T>
    int applyDirichletBC(apf::Numbering * nm,
                         T begin,
                         T end,
                         BCQuery * qry,
                         double t)
  {
    int fxd = 0;
    apf::Field * fld = apf::getField(nm);
    apf::Mesh * msh = apf::getMesh(fld);
    apf::FieldShape * fs = apf::getShape(fld);
    int cmps = apf::countComponents(fld);
    assert(qry->numComps() == cmps);
    for(T & it = begin; it != end; ++it)
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
}
