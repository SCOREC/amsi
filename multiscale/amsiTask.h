#ifndef AMSI_TASK_H_
#define AMSI_TASK_H_
#include "amsiMultiscaleConfig.h"
#include "amsiHost.h"
#include <PCU.h>
#include <amsiMPI.h>
#include <map>
#include <string>
namespace amsi
{
  class ProcessSet;
  class DataDistribution;
  typedef int(*ExecuteFunc)(int&,char**&,MPI_Comm);
  class Task
  {
  public:
    Task(ProcessSet* p);
    ~Task();
    int localRank() {return local_rank;}
    bool assignedTo() { return local_rank != -1; }
    int localToGlobalRank(int);
    int size();
    const MPI_Comm comm() {return task_comm;}
    const MPI_Group group() {return task_group;}
    void setExecutionFunction(ExecuteFunc e) {exec = e;}
    int execute(int& argc,char**& argv)
    {
      int result = 0;
      // don't execute of the global rank is not valid
      if(localToGlobalRank(localRank()) >= 0)
      {
        PCU_Switch_Comm(task_comm);
        result +=  (*exec)(argc,argv,task_comm);
      }
      return result;
    };
    size_t createDD(const std::string & nm);
    bool verifyDD(const std::string & nm);
    void setLocalDDValue(const std::string & nm, int ct);
    int getLocalDDValue(size_t dd_id);
    void assembleDD(const std::string & nm);
    size_t getDD_ID(const std::string & nm);
    DataDistribution * getDD(const std::string & dd_nm)
    {
      size_t dd_id = getDD_ID(dd_nm);
      return getDD(dd_id);
    }
    DataDistribution * getDD(size_t dd_id)
    {
      return data[dd_id]; // HACKY check it actually exists first...
    }
    const ProcessSet * getProcessSet() { return proc; }
  protected:
    MPI_Comm task_comm;
    MPI_Group task_group;
    ProcessSet * proc;
    typedef std::map<size_t,DataDistribution*> datamap_t;
    datamap_t data;
  private:
    int local_rank;
    ExecuteFunc exec;
    hash<std::string> id_gen;
  };
  int taskSize(Task * t);
}
#endif