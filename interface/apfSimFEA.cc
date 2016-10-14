#include "apfSimFEA.h"
#include "simAttributes.h"
#include "simBoundaryConditions.h"
#include "amsiMPI.h"
#include <apfSIM.h>
#include <apfSIMDataOf.h>
#include <apfShape.h>
#include <cassert>
namespace amsi
{
  apfSimFEA::apfSimFEA(pGModel imdl,
                       pParMesh imsh,
                       pACase pd,
                       MPI_Comm cm)
    : FEA(cm)
    , apfFEA(apf::createMesh(imsh),cm)
    , SimFEA(imdl,imsh,pd,cm)
    , mesh_size_field(NULL)
    , mesh_adapted(false)
  { }
  void apfSimFEA::ApplyBC_Dirichlet()
  {
    fixed_dofs = amsi::applySimDirichletBCs(apf_primary_numbering,
                                            part,
                                            dir_bcs.begin(),
                                            dir_bcs.end(),
                                            T);
    std::cout << "There are " << fixed_dofs << " dofs fixed by essential bound ary conditions." << std::endl;
  }
  void apfSimFEA::ApplyBC_Neumann(LAS * las)
  {
    assert(las);
    amsi::applySimNeumannBCs(las,
                             apf_primary_numbering,
                             part,
                             neu_bcs.begin(),
                             neu_bcs.end(),
                             T);
  }
  void apfSimFEA::Assemble(LAS * las)
  {
    ApplyBC_Neumann(las);
    apfFEA::Assemble(las);
  }
  void apfSimFEA::Adapt()
  {
    if(mesh_size_field != NULL)
    {
      sim_size_field =
        dynamic_cast<apf::SIMDataOf<double>* >(mesh_size_field->getData())->getSimField();
      SimFEA::Adapt();
    }
  }
  void apfSimFEA::addFieldToMap(apf::Field * fd)
  {
    apf::SIMDataOf<double> * field_data = dynamic_cast<apf::SIMDataOf<double>*>(fd->getData());
    if(field_data != NULL)
      SimFEA::addFieldToMap(field_data->getSimField());
  }
}

