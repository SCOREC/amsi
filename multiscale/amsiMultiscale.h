#ifndef AMSI_META_H_
#define AMSI_META_H_
#include "amsiCommunicationManager.h"
#include "amsiControlService.h"
#include "amsiMigration.h"
#include "amsiTaskManager.h"
#include <string>
namespace amsi
{
  void initMultiscale(int argc, char ** argv, MPI_Comm cm = MPI_COMM_WORLD);
  void freeMultiscale();
  void configureFromFile(const std::string &);
  extern TaskManager * tm;
  extern CommunicationManager * cm;
  extern bool from_file;
}
#endif
