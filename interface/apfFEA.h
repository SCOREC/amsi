#ifndef APFFEA_H_
#define APFFEA_H_
#include "FEA.h"
#include "amsiBCDirichlet.h"
#include "ElementalSystem.h"
#include "NeumannIntegrator.h"
#include <apf.h>
#include <apfField.h>
#include <apfMesh.h>
#include <apfNumbering.h>
namespace amsi
{
  namespace Analysis
  {
    apf::Field * analyzeMeshQuality(apf::Mesh * mesh, apf::Field * disp_field);
    class PrintField : public apf::FieldOp
    {
    private:
      apf::Field * f;
      apf::MeshEntity * me;
      std::ostream & os;
      int nc;
    public:
      PrintField(apf::Field * field, std::ostream & str) : f(field), me(), os(str), nc(0)
      {
        nc = f->countComponents();
      }
      virtual bool inEntity(apf::MeshEntity* e) {me = e; return true;}
      virtual void outEntity() {}
      virtual void atNode(int node)
      {
        double cmps[nc];
        apf::getComponents(f,me,node,&cmps[0]);
        for(int ii = 0; ii < nc; ii++)
          os << cmps[ii] << std::endl;
      }
    };
    class apfFEA : public virtual FEA
    {
    protected:
      apf::Mesh * apf_mesh;
      apf::Field * apf_primary_field;
      apf::Numbering * apf_primary_numbering;
      ElementalSystem * elemental_system;
      NeumannIntegrator * neumann_integrator;
      int Entity_ApplyBC_Dirichlet(apf::ModelEntity*,int,double);
      //void Entity_ApplyBC_Neumann(LAS*,apf::ModelEntity*,int);
    public:
    apfFEA() :
      FEA(MPI_COMM_WORLD,"[apf_fea]"),
        apf_mesh(NULL),
        apf_primary_field(NULL),
        apf_primary_numbering(NULL),
        elemental_system(NULL),
        neumann_integrator(NULL)
    {};
    apfFEA(MPI_Comm comm) :
      FEA(comm,"[apf_fea]"),
        apf_mesh(NULL),
        apf_primary_field(NULL),
        apf_primary_numbering(NULL),
        elemental_system(NULL),
        neumann_integrator(NULL)
    {};
    apfFEA(MPI_Comm comm,
           apf::Mesh * in_mesh) :
      FEA(comm,"[apf_fea]"),
        apf_mesh(in_mesh),
        apf_primary_field(NULL),
        apf_primary_numbering(NULL),
        elemental_system(NULL),
        neumann_integrator(NULL)
        {
          analysis_dim = apf_mesh->getDimension();
        };
      void RenumberDOFs();
      void ApplyBC_Dirichlet(std::vector<DirichletSpecification>&);
      void Assemble(LAS*);
      void UpdateDOFs(const double*);
      void WriteMesh(const std::string &);
    };
  }
}
#endif
