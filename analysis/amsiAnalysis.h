#ifndef AMSI_ANALYSIS_H_
#define AMSI_ANALYSIS_H_
#include "amsiFields.h"
#include "amsiAnalysisConfig.h"
#include "amsiUtil.h"
#include "amsiMPI.h"
#include "amsiPETScLAS.h"
#include "apfFields.h"
#include "simAttributes.h"
#include "simFields.h"
namespace amsi
{
  void initAnalysis(int argc, char ** argv, MPI_Comm cm = MPI_COMM_WORLD);
  void freeAnalysis();
# ifdef PETSC
  void petscInit(int argc, char ** argv, MPI_Comm cm = MPI_COMM_WORLD);
  void petscFree();
  extern bool use_petsc;
# endif
# ifdef SIM
  void simmetrixInit(int argc, char ** argv, MPI_Comm cm = MPI_COMM_WORLD);
  void simmetrixFree();
  extern bool use_simmetrix;
# endif
}
#endif
