#include "apfBoundaryConditions.h"
#include "apfField.h"
#include "apfShape.h"
#include "apfNumbering.h"
#include <iostream>
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
}
