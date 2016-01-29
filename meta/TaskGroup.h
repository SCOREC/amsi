#ifndef TASKGROUP_H_
#define TASKGROUP_H_

#include "Task.h"

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
