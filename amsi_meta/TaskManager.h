#ifndef TASKMANAGER_H_
#define TASKMANAGER_H_

#include "Task.h"
#include "TaskGroup.h"
#include "ProcessSet.h"
#include "amsiConfig.h"

#ifdef BGQ
  #include <boost/functional/hash.hpp>
#else
  #include <functional>
#endif

#include <mpi.h>

#include <string>

namespace amsi {

#ifdef BGQ
  using boost::hash;
#else
  using std::hash;
#endif

  Task * getLocal();
  
    /// The definition of the TaskManager class, which is used primarily to
    ///  create and manage tasks and generate unique ids for tasks and 
    ///  data distributions defined on tasks, and to manage task states for
    ///  any local tasks.
    class TaskManager{
    public:
      TaskManager(MPI_Comm);

      TaskGroup * TaskGroup_Create(const std::string & nm);
      TaskGroup * TaskGroup_Create(const std::string & nm, const std::vector<Task*>& t);
      size_t TaskGroup_GetID(const std::string & nm) const;
      TaskGroup * TaskGroup_Get(const std::string & nm);
      TaskGroup * TaskGroup_Get(size_t id);
      
      //Task * Task_Create(const std::string & nm, int s);
      Task * createTask(const std::string & nm, int s);
      
      // experimental
      void Task_AddOverride(const std::string & nm, Task * t);
      
      bool Task_Exists(const std::string & nm);
      
      Task * getTask(const std::string & nm);
      Task * Task_Get(size_t id);
      
      //size_t Task_GetID(const std::string & nm);
      size_t getTaskID(const std::string & nm);
      
      //Task * Task_GetLocal(); // won't work till after execute is called... makes sense I guess
      Task * getLocalTask();

      size_t DataDist_GetID(const std::string & t_nm, 
			    const std::string & dd_nm);

      bool LockConfig();
      int Execute(int&,char**&);
    protected:
    private:
      TaskManager();

      hash<std::string> id_gen;
      typedef std::map<size_t,Task*> taskmap_t;
      taskmap_t tasks;

      typedef std::map<size_t,TaskGroup*> taskgroupmap_t;
      taskgroupmap_t taskgroups;
      
      Task * local_task;
      ProcessSet * free_processes;
      bool config_locked;
    };

} // namespace amsi

#endif
