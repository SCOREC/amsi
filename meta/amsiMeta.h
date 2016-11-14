#ifndef AMSI_META_H_
#define AMSI_META_H_
#include "amsiTaskManager.h"
#include "amsiCommunicationManager.h"
#include <amsiFileSys.h>
#include <amsiUtil.h>
#include <string>
namespace amsi
{
  void metaInit(int argc, char ** argv, MPI_Comm cm = MPI_COMM_WORLD);
  void metaFree();
  void configureFromFile(const std::string &);
  extern TaskManager * tm;
  extern CommunicationManager * cm;
  extern FileSystemInfo * fs;
  extern bool from_file;
}
#endif
