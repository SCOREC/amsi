#include "amsiDeformation.h"
namespace amsi
{
  const apf::Matrix3x3 eye3x3(1.0,0.0,0.0,
                              0.0,1.0,0.0,
                              0.0,0.0,1.0);
  void deformationGradient(apf::Element * e, const apf::Vector3 & p, apf::Matrix3x3 & F)
  {
    apf::NewArray<apf::Vector3> u;
    apf::getVectorNodes(e,u);
    apf::NewArray<apf::Vector3> Ni_j;
    apf::getShapeGrads(e,p,Ni_j);
    F = eye3x3;
    int nen = apf::countNodes(e);
    int cmp = 3; //apf::countComponente(apf::getField(e));
    for(int kk = 0; kk < cmp; ++kk)
      for(int jj = 0; jj < cmp; ++jj)
        for(int ii = 0; ii < cmp; ++ii)
          F[kk][jj] += u[ii][kk] * Ni_j[ii][jj];
  }
}
