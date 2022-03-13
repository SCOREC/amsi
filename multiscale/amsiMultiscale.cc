#include "amsiMultiscale.h"
#include <pystring.h>
#include <fstream>
#include "amsiCommunicationManager.h"
#include "amsiControlService.h"
#include "amsiExceptions.h"
#include "amsiTaskManager.h"
namespace amsi {
  [[deprecated("use the Multiscale Struct")]] static TaskManager* tm = nullptr;
  [[deprecated("use Multiscale Struct")]] static CommunicationManager* cm =
      nullptr;
  [[deprecated("use Multiscale Struct")]] TaskManager* getScaleManager()
  {
    return tm;
  }
  [[deprecated("use Multiscale Struct")]] CommunicationManager*
  getMultiscaleManager()
  {
    return cm;
  }
  Multiscale::Multiscale(const MultiscaleOptions& options, const MPI& mpi)
      : task_manager_(std::make_unique<TaskManager>(mpi.getWorld().comm()))
      , communication_manager_(std::make_unique<CommunicationManager>())
      , mpi_(mpi)
  {
    // TODO add options for allocator for now default is fine since it's the
    // only thing I actually use. initialize TaskManager and Communication
    // Manager task_manager_ initialize multiscale analysis stuff set allocator
    // (defaults to ExclusiveProcess, need to also have Strided) -->refactor
    // process allocator to a lambda set scales
    // if (options.scales.size()  == 0)
    //{
    //  throw amsi_error{"Multiscale Analysis must have at least one scale in
    //  the configuration."};
    //}
    AMSI_COMM_SCALE = scale_.comm();
    for (const auto& scale : options.scales) {
      task_manager_->createTask(scale.name, scale.nprocs);
    }
    // set relations
    for (const auto& relation : options.relations) {
      auto t1 = task_manager_->getTaskID(relation.scale1);
      auto t2 = task_manager_->getTaskID(relation.scale2);
      if (!t1) {
        std::stringstream ss;
        ss << "configuration error: scale " << relation.scale1
           << " doesn't exist.";
        throw amsi_error{ss.str()};
      }
      if (!t2) {
        std::stringstream ss;
        ss << "configuration error: scale " << relation.scale2
           << " doesn't exist.";
        throw amsi_error{ss.str()};
      }
      communication_manager_->defineRelation(t1, t2);
      if (!communication_manager_->CommRelation_Exists(t1, t2)) {
        std::stringstream ss;
        ss << "configuration error: relation for " << relation.scale1 << " "
           << relation.scale2 << " not created";
        throw amsi_error{ss.str()};
      }
    }
    if (!options.scales.empty()) {
      if (!task_manager_->lockConfiguration()) {
        throw amsi_error{
            "ERROR: AMSI multiscale cannot configure with supplied options"};
      }
      scale_.set(task_manager_->getLocalTask()->comm());
      AMSI_COMM_SCALE = scale_.comm();
    }
    else {
      std::cout << "WARNING: configuring analysis without any scales. This is "
                   "most likely an error.\n";
    }
    // FIXME control service shouldn't be a singleton...
    control_service_ = ControlService::Instance();
    control_service_->SetTaskManager(task_manager_.get());
    control_service_->SetCommunicationManager(communication_manager_.get());
  }
  Multiscale::~Multiscale() = default;
  TaskManager* Multiscale::getScaleManager() const
  {
    return task_manager_.get();
  }
  CommunicationManager* Multiscale::getMultiscaleManager() const
  {
    return communication_manager_.get();
  }
  ControlService* Multiscale::getControlService() const
  {
    return control_service_;
  }
  const MPIComm& Multiscale::getCommScale() const { return scale_; }
  const MPIComm& Multiscale::getCommWorld() const { return mpi_.getWorld(); }
  const MPIComm& Multiscale::getCommSelf() const { return mpi_.getSelf(); }
  const MPI& Multiscale::getMPI() const { return mpi_; }
}  // namespace amsi
