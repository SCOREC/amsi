#ifndef ERRORESTIMATORS_H_
#define ERRORESTIMATORS_H_
#include <apfField.h>
#include <apfFieldOf.h>
#include <apfMesh.h>
#include <cassert>
namespace amsi
{
  template <typename T>
    T nodalAverage(apf::Mesh * msh,
                   apf::MeshEntity * me,
                   apf::Field * ipf);
  template <>
    apf::Matrix3x3 nodalAverage(apf::Mesh * msh,
                                apf::MeshEntity * me,
                                apf::Field * ipf)
  {
    apf::Matrix3x3 rslt;
    apf::Adjacent adj;
    msh->getAdjacent(me,msh->getDimension(),adj);
    int num_adj = adj.getSize();
    APF_ITERATE(apf::Adjacent,adj,eit)
    {
      apf::Matrix3x3 avg;
      apf::Element * e = apf::createElement(ipf,*eit);
      apf::NewArray<apf::Matrix3x3> fe_vals;
      apf::getMatrixNodes(e,fe_vals);
      int n = apf::countNodes(e);
      for(int ii = 0; ii < n; ii++)
        avg = avg + fe_vals[ii];
      avg = avg / n;
      rslt = rslt + avg;
    }
    rslt = rslt / num_adj;
    return rslt;
  }
  class RecoverNodalAverage : public apf::FieldOp
  {
  protected:
    apf::Mesh * mesh;
    apf::Field * ip_field;
    apf::Field * recovered_field;
    apf::MeshEntity * cme;
    int type;
  public:
  RecoverNodalAverage(apf::Field * ip_field,
                      apf::Field * in_recovered_field)
    : FieldOp()
      , mesh(apf::getMesh(in_recovered_field))
      , ip_field(NULL)
      , recovered_field(in_recovered_field)
      , cme(NULL)
      , type(0)
    {
      type = apf::getValueType(ip_field);
      assert(type == apf::getValueType(recovered_field));
    }
    bool inEntity(apf::MeshEntity * me)
    {
      cme = me;
      return true;
    }
    void outEntity() { }
    void atNode(int nd)
    {
      if(type == apf::MATRIX)
        apf::setMatrix(recovered_field,cme,nd,
                       nodalAverage<apf::Matrix3x3>(mesh,cme,ip_field));
    }
  };
}
#endif
