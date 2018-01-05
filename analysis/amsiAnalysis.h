#ifndef AMSI_ANALYSIS_H_
#define AMSI_ANALYSIS_H_
#include "amsiFields.h"
#include "amsiAnalysisConfig.h"
#include "amsiPETScLAS.h"
#include "apfFields.h"
#include "simAttributes.h"
#include "simFields.h"
#include "amsiNonlinearAnalysis.h"
namespace amsi
{
  #define ANALYSIS_CONFIG_SECTIONS(OP) OP(analysis), OP(num_analysis_config_sections)
  enum AnalysisConfigSections{ANALYSIS_CONFIG_SECTIONS(MAKE_ENUM_OP)};
  const char * getAnalysisSectionString(int ii);
  void initAnalysis(int argc, char ** argv, MPI_Comm cm = MPI_COMM_WORLD);
  void freeAnalysis();
#ifdef PETSC
  void usePetsc(const std::string & petsc_opts);
#endif
  void useSimmetrix(const std::string & sim_lic);
}
#endif
