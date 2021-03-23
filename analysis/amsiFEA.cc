#include "amsiFEA.h"
#include <algorithm>
#include <cassert>
#include <fstream>
#include <vector>
namespace amsi {
  FEA::FEA(std::shared_ptr<const mt::AssociatedModelTraits<mt::DimIdGeometry>> mt,
      std::vector<DirichletBCEntry> dbc, std::vector<NeumannBCEntry> nbc,
      MPI_Comm cm)
      : constraint_dofs(0)
      , local_dof_count(0)
      , first_local_dof(0)
      , global_dof_count(0)
      , fixed_dofs(0)
      , numbered(false)
      , T(0.0)
      , analysis_comm(cm)
      , model_traits(std::move(mt))
      , dirichlet_bcs(std::move(dbc))
      , neumann_bcs(std::move(nbc))
  {
  }
  void FEA::setSimulationTime(double t)
  {
    T = t;
    if(!PCU_Comm_Self())
      std::cout << "Simulation time updated: " << T << std::endl;
  }
  void FEA::GetDOFInfo(int& global, int& local, int& offset)
  {
    global = global_dof_count;
    local = local_dof_count;
    offset = first_local_dof;
  }
  void assembleMatrix(LAS* las, int rw_cnt, int* rw_nms, int cl_cnt,
                      int* cl_nms, double* Ke)
  {
    las->AddToMatrix(rw_cnt, rw_nms, cl_cnt, cl_nms, &Ke[0]);
  }
  void assembleVector(LAS* las, int rw_cnt, int* rw_nms, double* fe)
  {
    las->AddToVector(rw_cnt, rw_nms, &fe[0]);
  }
}  // namespace amsi
