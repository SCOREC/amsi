#ifndef APFFUNCTIONS_H_
#define APFFUNCTIONS_H_
#include <apf.h>
#include <apfDynamicVector.h>
#include <apfNumbering.h>
#include <ostream>
namespace amsi
{
  void faceNormal(apf::Mesh *,
                  apf::MeshEntity *,
                  apf::Vector3 & n);
  void vertexNormal(apf::Mesh *,
                    apf::MeshEntity *,
                    apf::Vector3 & n);
  void displaceMesh(apf::Field * displacement_field);
  void printNumbering(std::ostream & out, apf::Numbering * numbering);
  void SymmMatrixToVoigtVector(const apf::Matrix3x3 & mat, apf::Vector<6> & vec);
  void VoigtVectorToSymmMatrix(const apf::Vector<6> & vec, apf::Matrix3x3 & mat);
  void VoigtVectorToSymmMatrix(const apf::DynamicVector & vec, apf::Matrix3x3 & mat);
}
#endif
