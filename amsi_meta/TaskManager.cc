#include "Task.h"
#include "TaskManager.h"

#include "amsiMeta.h"
#include <amsiMPI.h>

#include <cassert>
#include <iostream>

namespace amsi {

  Task * getLocal()
  {
    return tm->getLocalTask();
  }

  TaskManager::TaskManager(MPI_Comm gc) :
    id_gen(),
    tasks(),
    local_task(),
    process_allocator(),
    config_locked(false)
  {
    int size = 0;
    MPI_Comm_size(gc,&size);

    process_allocator = static_cast<ProcessAllocator*>(new ExclusiveProcessAllocator(size));
  }
  
  TaskGroup * TaskManager::TaskGroup_Create(const std::string & nm)
  {
    TaskGroup * result = NULL;
    size_t id = id_gen(nm);
    if(taskgroups.find(id) != taskgroups.end())
      result = taskgroups[id] = new TaskGroup();
    return result;
  }

  TaskGroup * TaskManager::TaskGroup_Create(const std::string & nm,
					    const std::vector<Task*>& t)
  {
    TaskGroup * result = NULL;
    size_t id = id_gen(nm);
    if(taskgroups.find(id) != taskgroups.end())
      result = taskgroups[id] = new TaskGroup(t);
    return result;
  }

  size_t TaskManager::TaskGroup_GetID(const std::string & nm) const
  {
    size_t result = 0;
    result = id_gen(nm);
    assert(taskgroups.find(result) != taskgroups.end());
    return result;
  }

  
  
  Task * TaskManager::createTask(const std::string & nm, int s)
  {
    Task * result = NULL;
    size_t id = id_gen(nm);
    ProcessSet * assigned = process_allocator->assign(s,id);
    assert(assigned);
    result = tasks[id] = new Task(assigned);
    return result;
  }
  
  // experimental
  void TaskManager::Task_AddOverride(const std::string & nm, Task * t)
  {
    size_t id = id_gen(nm);
    tasks[id] = t;
  }
  
  bool TaskManager::Task_Exists(const std::string & nm)
  {
    std::map<size_t,Task*>::iterator it = tasks.find(id_gen(nm));
    return it != tasks.end();
  }
  
  Task * TaskManager::getTask(const std::string & nm)
  {
    std::map<size_t,Task*>::iterator it = tasks.find(id_gen(nm));
    return it != tasks.end() ? it->second : NULL;
  }
  
  Task * TaskManager::Task_Get(size_t id)
  {
    std::map<size_t,Task*>::iterator it = tasks.find(id);
    return it != tasks.end() ? it->second : NULL;
  }
  
  size_t TaskManager::getTaskID(const std::string & nm)
  {
    size_t result = id_gen(nm);
    std::map<size_t,Task*>::iterator it = tasks.find(result);
    if(it == tasks.end())
      result = 0;
    return result;
  }
  
  Task * TaskManager::getLocalTask()
  {
    return local_task;
  }
  
  size_t TaskManager::DataDist_GetID(const std::string & t_nm, 
				     const std::string & dd_nm)
  {
    size_t result = 0;
    size_t t_id = getTaskID(t_nm);
    if(t_id)
    {
      Task * t = tasks[t_id];
      result = t->DataDist_GetID(dd_nm);
    }
    return result;
  }

  bool TaskManager::lockConfiguration()
  {
    int rank = -1;
    MPI_Comm_rank(AMSI_COMM_WORLD,&rank);
    if(!config_locked)
    {
      if(process_allocator->isAssigned(rank))
      {
	for(std::map<size_t,Task*>::iterator it = tasks.begin(), itend = tasks.end(); it != itend; it++)
	{
	  if(it->second->proc->isIn(rank))
	    local_task = it->second;
	}
      }
      
      bool valid_config = true;
      if(valid_config)
	config_locked = true;
    }
    return config_locked;
  }
  
  int TaskManager::Execute(int& argc,char**& argv)
  {
    int result = 0;

    if(lockConfiguration())
      result = local_task->Execute(argc,argv);
    else
      result = -1;
    
    return result;
  }
} // namespace amsi
