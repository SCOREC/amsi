#include "amsiTaskGroup.h"
#include <algorithm> //find
namespace amsi
{
  TaskGroup::TaskGroup(const std::vector<Task*>& t)
    : tasks(t)
  {}
  bool TaskGroup::AddTask(Task * t)
  {
    if(InGroup(t))
      return false;
    else
      tasks.push_back(t);
    return true;
  }
  bool TaskGroup::RemoveTask(Task * t)
  {
    if(!InGroup(t))
      return false;
    else
      tasks.erase(std::find(tasks.begin(),tasks.end(),t));
    return true;
  }
  size_t TaskGroup::GroupSize()
  {
    return tasks.size();
  }
  bool TaskGroup::InGroup(Task * t)
  {
    return std::find(tasks.begin(),tasks.end(),t) != tasks.end();
  }
  Task * TaskGroup::operator[] (size_t i)
  {
    return tasks[i];
  }
}
