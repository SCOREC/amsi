#ifndef AMSIUTIL_H_
#define AMSIUTIL_H_
#include "amsiFail.h"
#include "amsiMPI.h"
#include "amsiEnumOps.h"
#include "amsiFileSys.h"
#include "amsiReporter.h"
#include "amsiString.h"
#include "amsiOutput.h"
#include "amsiUtilConfig.h"
namespace amsi
{
  #define UTIL_CONFIG_SECTIONS(OP) OP(util), OP(num_util_config_sections)
  enum UtilConfigSections{UTIL_CONFIG_SECTIONS(MAKE_ENUM_OP)};
  const char * getUtilConfigSectionString(int ii);
  const std::string & getOptionsFilename();
  bool configuredFromFile();
  extern FileSystemInfo * fs;
  void initUtil(int argc, char ** argv, MPI_Comm cm);
  void freeUtil();
  typedef unsigned long ulong;
}
#endif
