#ifndef TASK_H_
#define TASK_H_

#include "amsiConfig.h"

#ifdef BGQ
  #include <boost/functional/hash.hpp>
#else
  #include <functional>
#endif

#ifdef CORE
  #include <PCU.h>
#endif

#include <amsiMPI.h>

#include <map>
#include <string>

namespace amsi {

#ifdef BGQ
  using boost::hash;
#else
  using std::hash;
#endif
  

    // Forward declarations for interface definitions
    class ProcessSet;
    class DataDistribution;

    /// Type definition of the function signature required for the execution function registration for each task
    typedef int(*ExecuteFunc)(int&,char**&,MPI_Comm);

    // Prior to entering the adaptive phase of a simulation each task should set its status so that the adaptive systems know whether the model of the data/structure of the
    //  task may need to be adapted.
    
    /// Definition of the Task class which is used to model and manage execution of various simulation tasks/scales and important data associated with each tasks in parallel space. 
    class Task{
    public:
      Task(ProcessSet* p);
      ~Task();

      void SetExecutionFunction(ExecuteFunc e) {exec = e;};
      int Execute(int& argc,char**& argv)
      {
	int result = 0;
#       ifdef CORE
	PCU_Switch_Comm(task_comm);
#       endif

	result +=  (*exec)(argc,argv,task_comm);

#       ifdef CORE
	PCU_Switch_Comm(AMSI_COMM_WORLD);
#       endif
	return result;
      };

      void DataDist_Create(const std::string & nm);
      bool DataDist_Exists(const std::string & nm);
      void DataDist_SetLocal(const std::string & nm, int ct);
      //int DataDist_GetLocal(const std::string & nm); //not neaded atm
      int DataDist_GetLocal(size_t dd_id);
      void DataDist_Assemble(const std::string & nm);

      size_t DataDist_GetID(const std::string & nm);

      int LocalRank() {return local_rank;};
      int LocalToGlobalRank(int);

      int NumProcesses();
      MPI_Comm Comm() {return task_comm;};

      DataDistribution * DataDist_Get(const std::string & dd_nm)
      { 
	size_t dd_id = DataDist_GetID(dd_nm);
	return DataDist_Get(dd_id);
      }

      DataDistribution * DataDist_Get(size_t dd_id)
      {
	return data[dd_id]; // HACKY check it actually exists first...
      }

    protected:
      friend class TaskManager;
      friend class CommunicationManager;
      friend class ControlService;

      DataDistribution * operator[](const std::string & dd_nm)
      { 
	size_t dd_id = DataDist_GetID(dd_nm);
	return data[dd_id];
      }
 
      MPI_Comm task_comm;
      MPI_Group task_group;

      ProcessSet * proc;
      typedef std::map<size_t,DataDistribution*> datamap_t;
      datamap_t data;

    private:
      int rank;
      int local_rank;
      ExecuteFunc exec;

      hash<std::string> id_gen;
    };

} // namespace amsi

#endif
