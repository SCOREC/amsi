#ifndef AMSI_ANALYSIS_H_
#define AMSI_ANALYSIS_H_
#include "amsiLAS.h"
namespace amsi
{
  void assembleDOFs(LAS * las,
                    int nedofs,
                    int * dofids,
                    double * dofs,
                    double * ke,
                    double * fe,
                    bool induced_forces = false);
}
#endif
