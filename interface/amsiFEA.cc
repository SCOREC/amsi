#include "amsiFEA.h"
#include <algorithm>
#include <cassert>
#include <fstream>
#include <vector>
namespace amsi
{
  FEA::FEA(MPI_Comm cm)
    : constraint_dofs(0)
    , local_dof_count(0)
    , first_local_dof(0)
    , global_dof_count(0)
    , fixed_dofs(0)
    , numbered(false)
    , simulation_time(0.0)
    , analysis_comm(cm)
    , dir_bcs()
    , neu_bcs()
  { }
  void assembleMatrix(LAS * las,
                      int rw_cnt,
                      int * rw_nms,
                      int cl_cnt,
                      int * cl_nms,
                      double * Ke)
  {
    las->AddToMatrix(rw_cnt,rw_nms,cl_cnt,cl_nms,&Ke[0]);
  }
  void assembleVector(LAS * las,
                      int rw_cnt,
                      int * rw_nms,
                      double * fe)
  {
    las->AddToVector(rw_cnt,rw_nms,&fe[0]);
  }
}
