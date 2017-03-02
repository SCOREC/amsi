#ifndef AMSI_APFFEA_H_
#define AMSI_APFFEA_H_
#include "amsiFEA.h"
#include "amsiBoundaryConditions.h"
#include "apfFieldOp.h"
#include "ElementalSystem.h"
#include <apf.h>
#include <apfMesh.h>
#include <apfNumbering.h>
#include <iomanip>
namespace amsi
{
  apf::Field * analyzeMeshQuality(apf::Mesh * mesh, apf::Field * disp_field);
  class PrintField : public amsi::FieldOp
  {
  private:
    apf::Field * f;
    apf::MeshEntity * me;
    std::ostream & os;
    int nc;
  public:
  PrintField(apf::Field * field, std::ostream & str) : f(field), me(), os(str), nc(0)
    {
      nc = apf::countComponents(f);
      os << std::setprecision(16);
    }
    virtual bool inEntity(apf::MeshEntity* e) {me = e; return true;}
    virtual void outEntity() {}
    virtual void atNode(int node)
    {
      double cmps[nc];
      apf::getComponents(f,me,node,&cmps[0]);
      for(int ii = 0; ii < nc; ii++)
        os << cmps[ii] << std::endl;
      os << std::endl;
    }
    void run()
    {
      apply(f);
    }
  };
  class apfFEA : public virtual FEA
  {
  protected:
    apf::Mesh * apf_mesh;
    apf::Field * apf_primary_field;
    apf::Numbering * apf_primary_numbering;
    ElementalSystem * elemental_system;
  public:
    apfFEA(apf::Mesh * in_mesh, MPI_Comm cm = AMSI_COMM_SCALE)
      : FEA(cm)
      , apf_mesh(in_mesh)
      , apf_primary_field(NULL)
      , apf_primary_numbering(NULL)
      , elemental_system(NULL)
    {
      analysis_dim = apf_mesh->getDimension();
    };
    apf::Mesh * getMesh() { return apf_mesh; }
    virtual void RenumberDOFs();
    virtual void Assemble(LAS*);
    virtual void UpdateDOFs(const double*);
  };
}
#endif
