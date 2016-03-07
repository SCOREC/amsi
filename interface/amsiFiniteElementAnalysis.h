#ifndef AMSI_FINITE_ELEMENT_ANALYSIS_H_
#define AMSI_FINITE_ELEMENT_ANALYSIS_H_
#include "amsiAnalysis.h"
#include "amsiMPI.h"
namespace amsi
{
  void assembleDOFs(LAS * las,
                    int nedofs,
                    int * dofids,
                    double * dofs,
                    double * ke,
                    double * fe,
                    bool induced_forces = false);
};
#endif
