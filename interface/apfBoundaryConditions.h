#ifndef AMSI_APF_BOUNDARY_CONDITIONS_H_
#define AMSI_APF_BOUNDARY_CONDITIONS_H_
#include "amsiBoundaryConditions.h"
#include "apfMesh.h"
#include "apfField.h"
#include "apfShape.h"
#include "apfNumbering.h"
namespace amsi
{
  double getDirichletValue(BCQuery * qry,
                           apf::Mesh * msh,
                           apf::MeshEntity * ent,
                           int nd,
                           int cmp,
                           double t)
  {
    double val = 0.0;
    if(qry->isConst(cmp))
      val = qry->getValue(cmp);
    else
    {
      if(qry->isSpaceExpr(cmp))
      {
        // get parametric coordinate of nd
        apf::Vector3 pt;
        msh->getPoint(ent,nd,pt);
        // map local coord to global coord
        apf::Vector3 xyz;
        apf::mapLocalToGlobal(apf::createMeshElement(msh,ent),pt,xyz);
        // check if also time expr
        if(qry->isTimeExpr(cmp))
          val = qry->getValue(t,xyz[0],xyz[1],xyz[2]);
        else
          val = qry->getValue(xyz[0],xyz[1],xyz[2]);
      }
      else if(qry->isTimeExpr(cmp))
        val = qry->getValue(cmp,t);
      else
        std::cerr << "Error: Non constant dirichlet boundary condition with "
                  << " no temporal or spatial dependencies." << std::endl;
    }
    return val;
  }
  // assume that an iterator is supplied which iterates over all the mesh entities
  template <typename T>
    int applyDirichletBC(apf::Field * fld,
                         apf::Numbering * nm,
                         T begin,
                         T end,
                         BCQuery * qry,
                         double t)
  {
    int fxd = 0;
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
#endif
