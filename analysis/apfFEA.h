#ifndef AMSI_APFFEA_H_
#define AMSI_APFFEA_H_
#include <apf.h>
#include <apfMesh.h>
#include <apfNumbering.h>
#include <iomanip>
#include "ElementalSystem.h"
#include "amsiBoundaryConditionQuery.h"
#include "amsiFEA.h"
#include "apfFieldOp.h"
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
  class apfFEA : public FEA
  {
  protected:
    apf::Mesh * apf_mesh;
    apf::Field * apf_primary_field;
    apf::Field * apf_primary_delta_field;
    apf::Numbering * apf_primary_numbering;
    ElementalSystem * elemental_system;
    bool own_mesh;
  public:
    apfFEA(apf::Mesh * in_mesh,
           std::shared_ptr<const mt::AssociatedModelTraits<mt::DimIdGeometry>> mt,
           std::vector<DirichletBCEntry> dbc, std::vector<NeumannBCEntry> nbc,
         MPI_Comm cm = AMSI_COMM_SCALE, bool own_mesh=false)
      : FEA(std::move(mt), std::move(dbc), std::move(nbc), cm)
      , apf_mesh(in_mesh)
      , apf_primary_field(NULL)
      , apf_primary_delta_field(NULL)
      , apf_primary_numbering(NULL)
      , elemental_system(NULL)
      , own_mesh(own_mesh)
    {
      analysis_dim = apf_mesh->getDimension();
    };
    ~apfFEA()
    {
      if(own_mesh)
      {
        apf::destroyMesh(apf_mesh);
      }
    }
    apf::Mesh * getMesh() { return apf_mesh; }
    virtual void RenumberDOFs() override;
    virtual void Assemble(LAS*) override;
    virtual void UpdateDOFs(const double*) override;

    virtual void Adapt() override;
    virtual void ApplyBC_Dirichlet() override;
    virtual void ApplyBC_Neumann(LAS * las) override;
  };
}
#endif
