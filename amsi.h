#ifndef AMSI_AMSI_H
#define AMSI_AMSI_H
#include <PCU.h>
#include <mpi.h>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include "amsiAnalysis.h"
#include "amsiCommunicationManager.h"
#include "amsiConfigurationOptions.h"
#include "amsiControlService.h"
#include "amsiFileSys.h"
#include "amsiMPI.h"
#include "amsiMultiscale.h"
#include "amsiTaskManager.h"
namespace amsi {
  // replacement for singleton and global variables. This still isn't super nice
  // because we have this bag of all things amsi to pass around but at least
  // we are moving more towards testability that's impossible with the singleton
  class Amsi {
    public:
    // TODO need to remove global fs FileSystemInfo in initUtil then initUtil
    // can get deleted
    Amsi(const AmsiOptions& options, int argc, char** argv,
         MPI_Comm cm = MPI_COMM_WORLD)
        : mpi(argc, argv, cm)
        , results_directory_(
              std::make_unique<FileSystemInfo>(options.results_directory))
        , analysis(std::make_unique<Analysis>(options, argc, argv, cm))
    {
      if (options.multiscale_analysis) {
        multiscale = std::make_unique<Multiscale>(options, mpi);
      }
    }
    Amsi(const Amsi&) = delete;
    Amsi(Amsi&&) = delete;
    Amsi& operator=(const Amsi&) = delete;
    Amsi& operator=(const Amsi&&) = delete;
    // TODO remove usage of AMSI_COMM_SCALE and AMSI_COMM_WORLD
    // use the communicators here instead!
    MPI mpi;
    std::unique_ptr<Multiscale> multiscale;
    std::unique_ptr<Analysis> analysis;

    private:
    std::unique_ptr<FileSystemInfo> results_directory_;
    // potentially the multiscale doesn't exist?
    PCUScopeGuard pcu_scope_;
  };
}  // namespace amsi
#endif  // AMSI_AMSI_H
