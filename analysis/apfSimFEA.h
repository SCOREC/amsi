#ifndef APFSIMFEA_H_
#define APFSIMFEA_H_
#include "amsiMPI.h"
#include "apfFEA.h"
#include "SimFEA.h"
namespace amsi
{
  class apfSimFEA : public apfFEA, public SimFEA
  {
  protected:
    apf::Field * mesh_size_field;
    bool mesh_adapted;
  public:
    apfSimFEA(pGModel imdl,
              pParMesh imsh,
              pACase pd,
              MPI_Comm cm = AMSI_COMM_SCALE);
    virtual void Adapt();
    virtual void ApplyBC_Dirichlet();
    virtual void ApplyBC_Neumann(LAS * las);
    virtual void Assemble(LAS * las);
  private:
  };
}
#endif
