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
    virtual ~ProcessAllocator() {};
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
    virtual void addAssign(ProcessSet * set, size_t task_id)
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
  class StridedProcessAllocator : public ProcessAllocator
  {
  protected:
    ProcessSet_T<std::set<int>> * free_processes;
    int stride;
    size_t stride_task_id;
  public:
  StridedProcessAllocator(int size, int stride, size_t stride_task_id) :
    ProcessAllocator(size),
    stride(stride),
    stride_task_id(stride_task_id)
    {
    
      //free_processes = static_cast<ProcessSet*>(new ProcessSet_T<std::pair<int,int> >(0,size));
      free_processes = new ProcessSet_T<std::set<int>>(0,size);
    }
    ProcessSet * assign(int size, size_t task_id)
    {
      ProcessSet * result = NULL;
      if( size <= free_processes->size())
      {
        assert(stride_task_id);
        bool onStride = (task_id==stride_task_id);
        result = free_processes->extractStrided(size, stride, onStride);
        addAssign(result,task_id);
      }
      return result;
    }
    // use iterator based loops because index based loops will be
    // very slow for this operation
    virtual void addAssign(ProcessSet * set, size_t task_id)
    {
      ProcessSet_T<std::set<int>>::iterator it;
      for(int ii = 0; ii < set->size(); ii++)
        assignments[(*set)[ii]].push_back(task_id);
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
