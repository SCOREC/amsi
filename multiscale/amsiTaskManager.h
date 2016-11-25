#ifndef AMSI_TASKMANAGER_H_
#define AMSI_TASKMANAGER_H_
#include "amsiTask.h"
#include "amsiProcessSet.h"
#include "amsiProcessAllocator.h"
#include "amsiMultiscaleConfig.h"
#include "amsiHost.h"
#include "amsiScale.h"
#include "amsiLocality.h"
#include <amsiUUID.h>
#include <amsiRankSet.h>
#include <mpi.h>
#include <string>
namespace amsi
{
  Task * getLocal();
  /// The definition of the TaskManager class, which is used primarily to
  ///  create and manage tasks and generate unique ids for tasks and
  ///  data distributions defined on tasks, and to manage task states for
  ///  any local tasks.
  class TaskManager
  {
  public:
    TaskManager(MPI_Comm);
    Task * createTask(const std::string & nm, int s);
    Task * getTask(const std::string & nm);
    Task * Task_Get(size_t id);
    size_t getTaskID(const std::string & nm);
    Task * getLocalTask();
    size_t DataDist_GetID(const std::string & t_nm,
                          const std::string & dd_nm);
    bool lockConfiguration();
    int Execute(int&,char**&);
  protected:
  private:
    TaskManager();
    hash<std::string> id_gen;
    typedef std::map<size_t,Task*> taskmap_t;
    taskmap_t tasks;
    Task * local_task;
    ProcessAllocator * process_allocator;
    //ProcessSet * free_processes;
    bool config_locked;
  };
} // namespace amsi
#endif
