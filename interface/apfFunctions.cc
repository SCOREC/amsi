#include "apfFunctions.h"
#include "ElementalSystem.h"
#include <apfDynamicMatrix.h>
#include <apfField.h>
#include <apfGeometry.h>
#include <apfMesh.h>
#include <apfShape.h>
#include <cassert>
namespace amsi
{
  bool isFixed(int n)
  {
    return n == -2;
  }
  void faceNormal(apf::Mesh * msh, apf::MeshEntity * fc, apf::Vector3 & n)
  {
    assert(msh);
    assert(fc);
    n.zero();
    apf::Field * crds = msh->getCoordinateField();
    apf::Adjacent vs;
    msh->getAdjacent(fc,0,vs);
    apf::Vector3 vcrds[3];
    apf::getVector(crds,vs[0],0,vcrds[0]);
    apf::getVector(crds,vs[1],0,vcrds[1]);
    apf::getVector(crds,vs[2],0,vcrds[2]);
    apf::Plane p = apf::Plane::fromPoints(vcrds[0],vcrds[1],vcrds[2]);
    n = p.normal;
  }
  void vertexNormal(apf::Mesh * msh, apf::MeshEntity * vrt, apf::Vector3 & n)
  {
    assert(msh);
    assert(vrt);
    n.zero();
    apf::Adjacent adj;
    msh->getAdjacent(vrt,2,adj);
    APF_ITERATE(apf::Adjacent,adj,fc)
    {
      apf::Vector3 fn;
      faceNormal(msh,*fc,fn);
      n += fn;
    }
    n = n.normalize();
  }
  void displaceMesh(apf::Field * displacement_field)
  {
    apf::Mesh * mesh = apf::getMesh(displacement_field);
    apf::Field * apf_coord_field = mesh->getCoordinateField();
    for(int dim = 0; dim <= mesh->getDimension(); dim++)
    {
      apf::FieldShape * fs = apf::getShape(displacement_field);
      if(!fs->hasNodesIn(dim))
        break;
      apf::MeshIterator * it = mesh->begin(dim);
      while(apf::MeshEntity * me = mesh->iterate(it))
      {
        int num_nodes = fs->countNodesOn(mesh->getType(me));
        for(int jj = 0; jj < num_nodes; jj++)
        {
          apf::Vector3 disp, coord;
          apf::getVector(displacement_field,me,jj,disp);
          apf::getVector(apf_coord_field,me,jj,coord);
          apf::Vector3 new_coord = coord + disp;
          apf::setVector(apf_coord_field,me,jj,new_coord);
        }
      }
    }
  }
  void printNumbering(std::ostream & out, apf::Numbering * numbering)
  {
    apf::Mesh * mesh = apf::getMesh(numbering);
    apf::Field * field = apf::getField(numbering);
    int num_components = field->countComponents();
    apf::FieldShape * field_shape = apf::getShape(field);
    int analysis_dim = mesh->getDimension();
    for(int dim = 0; dim <= analysis_dim; dim++)
    {
      out << dim << " : " << std::endl;
      apf::MeshIterator * it = mesh->begin(dim);
      while(apf::MeshEntity * me = mesh->iterate(it))
      {
        if(mesh->isOwned(me))
        {
          int num_nodes = field_shape->countNodesOn(mesh->getType(me));
          for(int ii = 0; ii < num_nodes; ii++)
          {
            apf::Vector3 coord;
            mesh->getPoint(me,ii,coord);
            for(int jj = 0; jj < num_components; jj++)
            {
              if(apf::isNumbered(numbering,me,ii,jj))
                out << apf::getNumber(numbering,me,ii,jj) << "\t";
              else
                out << "F\t";
              out << coord[jj] << std::endl;
            }
          }
        }
      }
    }
  }
  void SymmMatrixToVoigtVector(const apf::Matrix3x3 & mat,
                               apf::Vector<6> & vec)
  {
    vec[0] = mat[0][0];
    vec[1] = mat[1][1];
    vec[2] = mat[2][2];
    vec[3] = mat[0][1];
    vec[4] = mat[1][2];
    vec[5] = mat[0][2];
  }
  void VoigtVectorToSymmMatrix(const apf::Vector<6> & vec,
                               apf::Matrix3x3 & mat)
  {
    mat[0][0] = vec[0];
    mat[1][1] = vec[1];
    mat[2][2] = vec[2];
    mat[0][1] = mat[1][0] = vec[3];
    mat[1][2] = mat[2][1] = vec[4];
    mat[0][2] = mat[2][0] = vec[5];
  }
  void VoigtVectorToSymmMatrix(const apf::DynamicVector & vec,
                               apf::Matrix3x3 & mat)
  {
    mat[0][0] = vec[0];
    mat[1][1] = vec[1];
    mat[2][2] = vec[2];
    mat[0][1] = mat[1][0] = vec[3];
    mat[1][2] = mat[2][1] = vec[4];
    mat[0][2] = mat[2][0] = vec[5];
  }
  class MeasureDisplaced : public amsi::ElementalSystem
  {
  public:
    MeasureDisplaced(apf::Field * field, int o)
      : ElementalSystem(field,o)
      , dim(0)
      , vol(0)
    {}
    void inElement(apf::MeshElement * me)
    {
      ElementalSystem::inElement(me);
      fs = apf::getShape(f);
      es = fs->getEntityShape(apf::getMesh(f)->getType(apf::getMeshEntity(me)));
      dim = apf::getDimension(me);
    }
    void atPoint(apf::Vector3 const &p, double w, double dV)
    {
      int & nen = nenodes; // = 4 (tets)
      // 1. Get coordinates on underlying mesh
      apf::Mesh * mesh = apf::getMesh(f);
      apf::Field * apf_coord_field = mesh->getCoordinateField();
      apf::Element * mesh_coord_elem = apf::createElement(apf_coord_field,me);
      apf::NewArray<apf::Vector3> mesh_xyz;
      apf::getVectorNodes(mesh_coord_elem,mesh_xyz);
      // 2. Get coordinates from apf_primary_field (passed in), which contains the accumulated displacement
      apf::NewArray<apf::Vector3> primary_field_xyz;
      apf::getVectorNodes(e,primary_field_xyz);
      // 3. Calculate current coordinates
      apf::DynamicMatrix xyz(nen,dim); xyz.zero();
      for (int ii = 0; ii < nen; ii++)
        for (int jj = 0; jj < dim; jj++)
          xyz(ii,jj) = mesh_xyz[ii][jj] + primary_field_xyz[ii][jj];
      // For Updated Lagrangian, the Jacobian of the updated coordinates are used
      // Note: that entires of Jacobian is hard coded for Linear tetrahedra elements.
      // TO DO: Generalize Jacobian for current configuration.
      apf::Matrix<3,3> J;
      J[0][0] = xyz(1,0) - xyz(0,0); // x2-x1
      J[0][1] = xyz(2,0) - xyz(0,0); // x3-x1
      J[0][2] = xyz(3,0) - xyz(0,0); // x4-x1
      J[1][0] = xyz(1,1) - xyz(0,1); // y2-y1
      J[1][1] = xyz(2,1) - xyz(0,1); // y3-y1
      J[1][2] = xyz(3,1) - xyz(0,1); // y4-y1
      J[2][0] = xyz(1,2) - xyz(0,2); // z2-z1
      J[2][1] = xyz(2,2) - xyz(0,2); // z3-z1
      J[2][2] = xyz(3,2) - xyz(0,2); // z4-z1
      double detJ = getDeterminant(J);
      vol = w * detJ;
    }
    double getVol(){return vol;}
  private:
    int dim;
    apf::FieldShape * fs;
    apf::EntityShape * es;
    double vol;
  };
  double measureDisplaced(apf::MeshEntity * ment, apf::Field * u)
  {
    //todo : derive integration order from field order
    MeasureDisplaced elemental_volume(u,1);
    apf::Mesh * msh = apf::getMesh(u);
    apf::MeshElement * mlm = apf::createMeshElement(msh,ment);
    elemental_volume.process(mlm);
    double volume = elemental_volume.getVol();
    apf::destroyMeshElement(mlm);
    return volume;
  }
}
