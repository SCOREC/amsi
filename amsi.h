#ifndef AMSI_AMSI_H
#define AMSI_AMSI_H
#include <mpi.h>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include "amsiAnalysis.h"
#include "amsiCommunicationManager.h"
#include "amsiConfigurationOptions.h"
#include "amsiControlService.h"
#include "amsiMPI.h"
#include "amsiMultiscale.h"
#include "amsiTaskManager.h"
#include <optional>
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
        : mpi_(argc, argv, cm)
    {
      if (options.multiscale) {
        multiscale_.emplace(options.multiscale.value(), mpi_);
      }
    }
    Amsi(const Amsi&) = delete;
    Amsi(Amsi&&) = delete;
    Amsi& operator=(const Amsi&) = delete;
    Amsi& operator=(const Amsi&&) = delete;

    private:
    MPI mpi_;

    public:
    const std::optional<Multiscale>& getMultiscale() const
    {
      return multiscale_;
    }

    private:
    std::optional<Multiscale> multiscale_;
  };
}  // namespace amsi
#endif  // AMSI_AMSI_H
