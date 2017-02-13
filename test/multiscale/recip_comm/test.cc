#include "test.h"
#include "amsiMultiscale.h"
#include "amsiControlService.h"
#include <mpi.h>
#include <iostream>
#include <utility>
using namespace amsi;
const int vsize = 6;
// Test data types for communication
MPI_Datatype sigma_type;
struct Sigma
{
  double v[vsize];
  bool operator==(Sigma & other)
    {
      bool result = true;
      for(int i = 0; i < vsize && result; i++)
        result = other.v[i] == v[i];
      return result;
    }
};
int task1_run(int &, char **&, MPI_Comm)
{
  int failed = 0;
  // Retrieve the ControlService
  ControlService * cs = ControlService::Instance();
  TaskManager * tm = cs->GetTaskManager();
  Task * lt = tm->getLocalTask();
  int local_rank = lt->localRank();
  int local_data_count = 6+local_rank*2;
  // Create a DataDistribution for this task, set the local data count, and assemble
  lt->createDD("micro_init");
  lt->setLocalDDValue("micro_init",local_data_count);
  lt->assembleDD("micro_init");
  // Create a CommPattern to communicate from this task to the other
  size_t pattern_id = cs->CreateCommPattern("micro_init","macro","micro");
  failed += test_neq(".CreateCommPattern()",static_cast<size_t>(0),pattern_id);
  // Reconcile the CommPattern with the other task
  cs->CommPattern_Reconcile(pattern_id);
  // Create data to communicate
  std::vector<Sigma> data(local_data_count);
  for(int ii = 0; ii < local_data_count; ii++){
    for(int jj=0;jj<vsize;jj++){
      data[ii].v[jj] = 0.5*(local_rank+1) + ii + jj;
      //data[ii].v[5] = data[ii].v[4] = data[ii].v[3] = data[ii].v[2] = data[ii].v[1] = data[ii].v[0] = 0.5;
    }
  }
  // Perform the communication with the created pattern and the data buffer
  cs->Communicate(pattern_id,data,sigma_type);
  //std::cout << "Data size: " << data.size() << " content: " << data << std::endl;
  //std::cout << "Task 1 rank " << local_rank << " has sent all data" << std::endl;
  // Create a placeholder CommPattern to reconcile into
  size_t recv_pattern_id = cs->RecvCommPattern("micro_results",
                                               "micro",
                                               "",
                                               "macro");
  // Reconcile the CommPattern from the other task to this one
  cs->CommPattern_Reconcile(recv_pattern_id);
  // Recv data from the other task based on the just-reconciled CommPattern
  std::vector<Sigma> result_data;
  cs->Communicate(recv_pattern_id,result_data,sigma_type);
//  bool areequal;
  // Check the data
  if(data.size() == result_data.size())
  {
    for(unsigned int ii = 0; ii < data.size(); ii++)
    {
      if(data[ii] == result_data[ii]){
        continue;
      }
      else{
        std::cout << "Rank " << local_rank << " : inconsistent data at index " << ii << std::endl;
        for(int jj=0;jj<vsize;jj++){
          std::cout<<"  "<<data[ii].v[jj]<<" ?= "<<result_data[ii].v[jj]<<" | ";
        }
        std::cout<<std::endl;
      }
    }
  }
  else{
    std::cout<<"Rank:"<<local_rank<<std::endl;
    std::cout<<"data:"<<std::endl;
    for(unsigned ii=0;ii<data.size();ii++){
      for(int jj=0;jj<vsize;jj++){
        std::cout<<"  "<<data[ii].v[jj];
      }
      std::cout<<std::endl;
    }
    std::cout<<"result_data:"<<std::endl;
    for(unsigned ii=0;ii<result_data.size();ii++){
      for(int jj=0;jj<vsize;jj++){
        std::cout<<"  "<<result_data[ii].v[jj];
      }
      std::cout<<std::endl;
    }
  }
  failed += test("2-Way Communication",data.size(),result_data.size());
  //std::cout << "Task 1 rank " << local_rank << " has recv'd all data" << std::endl;
  return failed;
}
int task2_run(int &, char **&, MPI_Comm)
{
  int failed = 0;
  // Retrieve the ControlService
  ControlService * cs = ControlService::Instance();
  TaskManager * tm = cs->GetTaskManager();
  Task * lt = tm->getLocalTask();
  //int local_rank = lt->localRank();
  // Create a placeholder CommPattern to reconcile into
  size_t pattern_id = cs->RecvCommPattern("micro_init","macro","","micro");
  failed += test_neq(".RecvCommPattern",static_cast<size_t>(0),pattern_id);
  // Reconcile the CommPattern from the other task
  cs->CommPattern_Reconcile(pattern_id);
  // Recv data
  std::vector<Sigma> data;
  cs->Communicate(pattern_id,data,sigma_type);
  //invert the comm mapping for use as the commpattern for the t2->t1 relation
  lt->createDD("micro_results");
  lt->setLocalDDValue("micro_results",data.size());
  lt->assembleDD("micro_results");
  // invert the communication pattern used to get in the t1->t2 "send_to_task2" relation
  size_t send_pattern_id = cs->CommPattern_UseInverted(pattern_id,"micro_results","micro","macro");
  //std::cout << "recv pattern id is " << pattern_id << " send pattern id is " << send_pattern_id << std::endl;
  // assemble the pieces of the inverted pattern so each process has the full pattern (currently required for reconciliation)
  cs->CommPattern_Assemble(send_pattern_id); // collective on the task
  cs->CommPattern_Reconcile(send_pattern_id); // collective on the relation closure
  cs->Communicate(send_pattern_id,data,sigma_type); // collective on the relation closure
  //std::cout << "Task 2 rank " << local_rank << " has sent all data" << std::endl;
  return failed;
}
int main(int argc, char * argv[])
{
  int failed = 0;
  amsi::initMultiscale(argc,argv);
  std::cout << "Initializing test object(s):" << std::endl;
  // create an MPI datatypes used in the simulation as coupling data
  MPI_Type_contiguous(vsize,MPI_DOUBLE,&sigma_type);
  // Need to commit this new type
  MPI_Type_commit(&sigma_type);
  // find size of sigma_type
  int sigmaTypeSize;
  MPI_Type_size(sigma_type,&sigmaTypeSize);
  Task * t1 = amsi::getScaleManager()->getTask("macro");
  Task * t2 = amsi::getScaleManager()->getTask("micro");
  ControlService * cs = ControlService::Instance();
  failed += test_neq("ControlService::Instance()",static_cast<ControlService*>(NULL),cs);
  // Set the execution functions for the tasks
  t1->setExecutionFunction(&task1_run);
  t2->setExecutionFunction(&task2_run);
  // Begin execution
  failed += cs->Execute(argc,argv);
  test("Number failed",0,failed);
  amsi::freeMultiscale();
  return failed;
}
