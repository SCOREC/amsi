#ifndef AMSI_PROCESSALLOCATOR_H_
#define AMSI_PROCESSALLOCATOR_H_
#include <cassert>
#include <vector>
namespace amsi
{
  class ProcessAllocator
  {
  protected:
    ProcessSet * managed_processes;
    std::vector<std::vector<size_t> > assignments;
  public:
  ProcessAllocator(int size) :
    assignments(size)
    {
      managed_processes = static_cast<ProcessSet*>(new ProcessSet_T<std::pair<int,int> >(0,size));
    }
    bool isAssigned(int rank)
    {
      assert(rank < managed_processes->size());
      return assignments[rank].size() > 0;
    }
    void getAssignments(int rank, std::vector<size_t> & a)
    {
      if(isAssigned(rank))
        a = assignments[rank];
    }
    virtual ProcessSet * assign(int size, size_t task_id) = 0;
    void addAssign(ProcessSet * set, size_t task_id)
    {
      for(int ii = 0; ii < set->size(); ii++)
        assignments[(*set)[ii]].push_back(task_id);
    }
  };
  class ExclusiveProcessAllocator : public ProcessAllocator
  {
  protected:
    ProcessSet * free_processes;
  public:
  ExclusiveProcessAllocator(int size) :
    ProcessAllocator(size)
    {
      free_processes = static_cast<ProcessSet*>(new ProcessSet_T<std::pair<int,int> >(0,size));
    }
    ProcessSet * assign(int size, size_t task_id)
    {
      ProcessSet * result = NULL;
      if( size <= free_processes->size() )
      {
        result = free_processes->extract(size);
        addAssign(result,task_id);
      }
      return result;
    }
  };
  class OverlapProcessAllocator : public ProcessAllocator
  {
  public:
  OverlapProcessAllocator(int size) : ProcessAllocator(size)
    {}
    ProcessSet * assign(int size, size_t task_id)
    {
      ProcessSet * result = NULL;
      if( size <= managed_processes->size() )
      {
        int low_rank = (*managed_processes)[0];
        int high_rank = (*managed_processes)[size];
        result = static_cast<ProcessSet*>(new ProcessSet_T<std::pair<int,int> >(low_rank,high_rank));
        addAssign(result,task_id);
      }
      return result;
    }
  };
}
#endif
