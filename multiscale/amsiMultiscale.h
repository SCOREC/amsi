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
    Multiscale(const AmsiOptions& options, MPI& mpi);
    [[nodiscard]] TaskManager* getScaleManager() const;
    [[nodiscard]] CommunicationManager* getMultiscaleManager() const;
    ControlService* getControlService() const;
    // destructor in c++ file so we can unique_ptr with incomplete types.
    ~Multiscale();

    private:
    std::unique_ptr<TaskManager> task_manager_;
    std::unique_ptr<CommunicationManager> communication_manager_;
    ControlService* control_service_;
    [[maybe_unused]] PCUScopeGuard pcu_scope_guard_;
  };
}  // namespace amsi
#endif
