#ifndef AMSIUTIL_H_
#define AMSIUTIL_H_
#include "amsiMPI.h"
#include "amsiEnumOps.h"
#include "amsiFileSys.h"
#include "amsiReporter.h"
#include "amsiOutput.h"
#include "amsiUtilConfig.h"
namespace amsi
{
  #define UTIL_CONFIG_SECTIONS(OP) OP(util), OP(num_util_config_sections)
  enum UtilConfigSections{UTIL_CONFIG_SECTIONS(MAKE_ENUM_OP)};
  const char * const UtilConfigSectionStrings[] = {UTIL_CONFIG_SECTIONS(MAKE_STRING_OP)};
  extern FileSystemInfo * fs;
  extern bool cnfg_frm_fl;
  extern std::string options_filename;
  void initUtil(int argc, char ** argv, MPI_Comm cm = MPI_COMM_WORLD);
  void freeUtil();
}
#endif
