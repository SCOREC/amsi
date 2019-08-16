#ifndef AMSI_META_H_
#define AMSI_META_H_
#include "amsiCommunicationManager.h"
#include "amsiControlService.h"
#include "amsiMigration.h"
#include "amsiTaskManager.h"
#include <amsiEnumOps.h>
#include <string>
namespace amsi
{
  #define MULTISCALE_CONFIG_SECTIONS(OP) OP(scales), OP(relations), OP(allocator), OP(num_multiscale_config_sections)
  enum MultiscaleConfigSections{MULTISCALE_CONFIG_SECTIONS(MAKE_ENUM_OP)};
  const char * const MultiscaleConfigSectionStrings[] = {MULTISCALE_CONFIG_SECTIONS(MAKE_STRING_OP)};
  void initMultiscale(int argc, char ** argv, MPI_Comm cm);
  void freeMultiscale();
  TaskManager * getScaleManager();
  CommunicationManager * getMultiscaleManager();
  extern bool from_file;
}
#endif
