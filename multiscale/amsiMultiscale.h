#ifndef AMSI_META_H_
#define AMSI_META_H_
#include <amsiEnumOps.h>
#include <sstream>
#include <string>
#include "amsiConfigurationOptions.h"
#include "amsiMPI.h"
namespace amsi {
  class ControlService;
  class TaskManager;
  class CommunicationManager;
  struct Multiscale {
    Multiscale(const MultiscaleOptions& options, const MPI& mpi);
    [[nodiscard]] TaskManager* getScaleManager() const;
    [[nodiscard]] CommunicationManager* getMultiscaleManager() const;
    ControlService* getControlService() const;
    // destructor in c++ file so we can unique_ptr with incomplete types.
    ~Multiscale();
    [[nodiscard]] const MPIComm& getCommScale() const;
    [[nodiscard]] const MPIComm& getCommWorld() const;
    [[nodiscard]] const MPIComm& getCommSelf() const;
    [[nodiscard]] const MPI& getMPI() const;

    private:
    std::unique_ptr<TaskManager> task_manager_;
    std::unique_ptr<CommunicationManager> communication_manager_;
    ControlService* control_service_;
    const MPI& mpi_;
    MPIComm scale_;
  };
}  // namespace amsi
#endif
