#include "apfFunctions.h"

#include <apfField.h>
#include <apfMesh.h>
#include <apfShape.h>

namespace amsi {
  namespace Analysis {

    bool isFixed(int dof_num)
    {
      return dof_num == -2;
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
      for(int dim = 0; dim < analysis_dim; dim++)
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
  }
}
