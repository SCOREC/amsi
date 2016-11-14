#ifndef AMSI_TASKGROUP_H_
#define AMSI_TASKGROUP_H_
#include "amsiTask.h"
#include <string>
#include <vector>
namespace amsi {
  class TaskGroup
  {
  private:
    std::string name;
    std::vector<Task*> tasks;
  public:
    TaskGroup() {};
    TaskGroup(const std::vector<Task*>& t);
    bool AddTask(Task*);
    bool RemoveTask(Task*);
    size_t GroupSize();
    bool InGroup(Task*);
    Task * operator[] (size_t i);
  };
}
#endif
