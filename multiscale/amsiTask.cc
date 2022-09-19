#include "amsiTask.h"
#include "amsiDataDistribution.h"
#include "amsiProcessSet.h"
#include "amsiMultiscale.h"
#ifdef ZOLTAN
#include <zoltan.h>
#endif
namespace amsi
{
  int taskSize(Task * t)
  {
    const ProcessSet * ps = t->getProcessSet();
    return ps->size();
  }
  /// @brief Standard constructor, collective on the ProcessSet passed in
  /// @param p The ProcessSet describing the parallel ranks on which the Task will execute
  Task::Task(ProcessSet * p)
    : task_comm()
    , task_group()
    , proc(p)
    , data()
    , local_rank(-1)
    , id_gen()
  {
    // Create a comm for the task
    MPI_Group world_group;
    MPI_Comm_group(AMSI_COMM_WORLD,&world_group);
    int n = p->size();
    int pr[n];
    toArray(p,&pr[0]);
    MPI_Group_incl(world_group,n,pr,&task_group);
    MPI_Comm_create(AMSI_COMM_WORLD,task_group,&task_comm);
    if(task_comm != MPI_COMM_NULL)
      MPI_Comm_rank(task_comm,&local_rank);
    else
    {
      int gbl_rnk = -1;
      MPI_Comm_rank(AMSI_COMM_WORLD,&gbl_rnk);
      local_rank = proc->indexOf(gbl_rnk);
    }
  }
  Task::~Task()
  {
    delete proc;
    for(datamap_t::iterator it = data.begin(), itend = data.end(); it != itend; it++)
      delete (it->second);
  }
# ifdef ZOLTAN
  // hacky hard-coded bullshit
  int DD_get(void * data, int *)
  {
    DataDistribution * dd = NULL;
    int local_rank = -1;
    size_t s1 = sizeof(DataDistribution*);
    size_t s2 = sizeof(int);
    memcpy(&dd,data,s1);
    memcpy(&local_rank,(void*)(((size_t)data)+s1),s2);
    /*
      for(size_t ii = 0; ii < s1; ii++)
      ((char*)&dd)[ii] = ((char*)data)[ii];
      for(size_t ii = s1; ii < s1+s2; ii++)
      ((char*)&local_rank)[ii] = ((char*)data)[ii];
    */
    assert(dd != NULL);
    assert(local_rank >= 0);
    return (*dd)[local_rank];
  }
  void DD_describe(void * data,
                   int,
                   int,
                   ZOLTAN_ID_PTR global_ids,
                   ZOLTAN_ID_PTR local_ids,
                   int,
                   float * obj_wgts,
                   int *)
  {
    DataDistribution * dd = NULL;
    int local_rank = -1;
    size_t s1 = sizeof(DataDistribution*);
    size_t s2 = sizeof(int);
    memcpy(&dd,data,s1);
    memcpy(&local_rank,(void*)(((size_t)data)+s1),s2);
    /*
      for(size_t ii = 0; ii < s1; ii++)
      ((char*)&dd)[ii] = ((char*)data)[ii];
      for(size_t ii = s1; ii < s1+s2; ii++)
      ((char*)&local_rank)[ii] = ((char*)data)[ii];
    */
    assert(dd != NULL);
    assert(local_rank >= 0);
    int offset = 0;
    for(int ii = 0; ii < local_rank; ii++)
      offset += (*dd)[ii];
    for(int ii = 0; ii < (*dd)[local_rank]; ii++)
    {
      local_ids[ii] = ii;
      global_ids[ii] = offset + ii;
      obj_wgts[ii] = dd->getWeight(local_rank,ii);
    }
  }
#endif
  void Task::registerDataDistribution(const std::string & nm,
                                      DataDistribution * dd)
  {
    data[getDD_ID(nm)] = dd;
  }
  /// @brief Determine whether a DataDistribution with the given name exists
  /// @param nm A unique string identifying a DataDistribution (maybe)
  /// @return bool Whether a DataDistribution with the given name exists
  bool Task::verifyDD(const std::string & nm)
  {
    bool exists = false;
    if(assignedTo())
      exists = data.count(getDD_ID(nm)) > 0;
    return exists;
  }
  /// @brief Get a DataDistribution's identifier from the unique name
  /// @param nm A string uniquely identifying the DataDistribution
  /// @return size_t Identifier for the named DataDistribution
  size_t Task::getDD_ID(const std::string &nm)
  {
    size_t result = id_gen(nm);
    return result;
  }
  /// @brief Get the global rank of a local rank
  /// @param r The local rank to translate to global rank
  /// @note May need to relocate this
  int Task::localToGlobalRank(int r) { return (*proc)[r]; }
  int Task::size() { return proc->size(); }
  int Task::execute(int &argc, char **&argv, Multiscale &multiscale)
  {
    int result = 0;
    // don't execute of the global rank is not valid
    if (localToGlobalRank(localRank()) >= 0) {
      MPI_Comm cm = PCU_Get_Comm();
      PCU_Switch_Comm(task_comm);
      result += (*exec)(argc, argv, task_comm, multiscale);
      PCU_Switch_Comm(cm);
    }
    return result;
  }
  DataDistribution *createDataDistribution(Task *tsk, const std::string &nm)
  {
    DataDistribution *dd = NULL;
    int sz = tsk->size();
    int rnk = tsk->localRank();
#ifndef ZOLTAN
    dd = new DataDistribution(sz, rnk, true);
#else
    MPI_Comm cm = tsk->comm();
    if (!sz)
      dd = new DataDistribution(sz, rnk);
    else {
      Zoltan_Struct *zs = Zoltan_Create(cm);
      dd = new DataDistribution(sz, rnk, true, zs);
      size_t s1 = sizeof(DataDistribution *);
      size_t s2 = sizeof(int);
      void *buffer = (void *)new char[s1 + s2];
      memcpy(buffer, &dd, s1);
      memcpy((void *)(((size_t)buffer) + s1), &rnk, s2);
      Zoltan_Set_Fn(zs, ZOLTAN_NUM_OBJ_FN_TYPE, (void (*)()) & DD_get, buffer);
      Zoltan_Set_Fn(zs, ZOLTAN_OBJ_LIST_FN_TYPE, (void (*)()) & DD_describe,
                    buffer);
    }
#   endif
    tsk->registerDataDistribution(nm,dd);
    return dd;
  }
} // namespace amsi
