// in-build
#include "../../test.h"

// in-project
#include "ControlService.h"
#include "amsiUtil.h"

// standard
#include <iostream>
#include <utility>

using namespace amsi;

int task1_run(int &, char **&, MPI_Comm)
{
  int failed = 0;

  // Retrieve the ControlService singleton object
  ControlService * cs = ControlService::Instance();
  TaskManager * tm = cs->GetTaskManager();

  // Get the local rank and determine an amount of locally-owned data
  int local_rank = tm->getLocalTask()->LocalRank();
  int local_data_count = (local_rank % 4) * 4;

  // Create a DataDistribution, set the local data, and assemble the DD on the local Task
  tm->getLocalTask()->DataDist_Create("send_to_task2");
  tm->getLocalTask()->DataDist_SetLocal("send_to_task2",local_data_count);
  tm->getLocalTask()->DataDist_Assemble("send_to_task2");

  // Create a CommPattern for the "send_to_task2" DataDistribution
  size_t pattern_id = cs->CreateCommPattern("send_to_task2","task1","task2");
  //cs->CommPattern_AssembleLocal(pattern_id);

  failed += test_neq(".CreateCommPattern()",static_cast<size_t>(0),pattern_id);

  // Reconcile the data distribution between the task's ProcessSets
  cs->CommPattern_Reconcile(pattern_id);

  // Create local data
  std::vector<double> data(local_data_count);
  for(int ii = 0; ii < local_data_count; ii++)
  {
    data[ii] =  static_cast<double>(local_data_count * ii) / static_cast<double>(local_rank);
  }

  // Send the local data to task2
  cs->Communicate(pattern_id,data,MPI_DOUBLE);

  //std::cout << "Data size: " << data.size() << " content: " << data << std::endl;
  //std::cout << "Task 1 rank " << local_rank << " has sent all data" << std::endl;
  
  // Create a placeholder communication pattern to reconcile into
  size_t recv_pattern_id = cs->RecvCommPattern("send_to_task1","task2","task1","");
  
  // Reconcile the communication pattern from task2 to this task
  cs->CommPattern_Reconcile(recv_pattern_id);

  // Create a recv buffer and communicate
  std::vector<double> result_data;
  cs->Communicate(recv_pattern_id,result_data,MPI_DOUBLE);

  // Check the recved data against the sent data
  if(data.size() == result_data.size())
  {
    for(unsigned int ii = 0; ii < data.size(); ii++)
    {
      if(data[ii] == result_data[ii])
     	continue;
      else
	std::cout << "Rank " << local_rank << " : inconsistent data at index " << ii << std::endl;
      
    }
  }
  failed += test("2-Way Communication",data.size(),result_data.size());

  //std::cout << "Task 1 rank " << local_rank << " has recv'd all data" << std::endl;
 
  return failed;
}


int task2_run(int &, char **&, MPI_Comm)
{
  int failed = 0;

  // Retrieve the ControlService object
  ControlService * cs = ControlService::Instance();
  TaskManager * tm = cs->GetTaskManager();
  
  int local_rank = tm->getLocalTask()->LocalRank();

  // Create a placeholder CommPattern to reconcile into
  size_t pattern_id = cs->RecvCommPattern("send_to_task2","task1","task2","");
  
  failed += test_neq(".RecvCommPattern",static_cast<size_t>(0),pattern_id);

  // Reconcile the CommPattern from task1 to this task
  cs->CommPattern_Reconcile(pattern_id);

  // Recv data from task1
  std::vector<double> data;
  cs->Communicate(pattern_id,data,MPI_DOUBLE);

  //invert the comm mapping for use as the commpattern for the t2->t1 relation
  tm->getLocalTask()->DataDist_Create("send_to_task1");
  tm->getLocalTask()->DataDist_SetLocal("send_to_task1",data.size());
  tm->getLocalTask()->DataDist_Assemble("send_to_task1");

  // invert the communication pattern used to get in the t1->t2 "send_to_task2" relation
  size_t send_pattern_id = cs->CommPattern_UseInverted(pattern_id,"send_to_task1","task2","task1");

  //std::cout << "recv pattern id is " << pattern_id << " send pattern id is " << send_pattern_id << std::endl;

  // assemble the pieces of the inverted pattern so each process has the full pattern (currently required for reconciliation)
  cs->CommPattern_Assemble(send_pattern_id); // collective on the task
  cs->CommPattern_Reconcile(send_pattern_id); // collective on the relation closure
  cs->Communicate(send_pattern_id,data,MPI_DOUBLE); // collective on the relation closure

  //std::cout << "Task 2 rank " << local_rank << " has sent all data" << std::endl;

  return failed;
}

int main(int argc, char * argv[])
{
  int failed = 0;
  MPI_Init(&argc,&argv);

  int rank = -1;
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);

  int size;
  MPI_Comm_size(MPI_COMM_WORLD,&size);

  std::cout << "Initializing test object(s):" << std::endl;

  // Create a TaskManager and the two tasks, task1 with 8 processes and task2 with 24 processes
  TaskManager * tm = new TaskManager(MPI_COMM_WORLD);
  Task * t1 = tm->createTask("task1",8);
  Task * t2 = tm->createTask("task2",24);

  CommunicationManager * cm = new CommunicationManager();
  ControlService * cs = ControlService::Instance();
  
  // Register the CommunicationManager and TaskManager with the ControlService
  cs->SetCommunicationManager(cm);
  cs->SetTaskManager(tm);

  failed += test_neq("ControlService::Instance()",static_cast<ControlService*>(NULL),cs);

  // Define CommRelations from task1 to task2 and from task2 to task1
  cs->CommRelation_Define("task1","task2");
  cs->CommRelation_Define("task2","task1");

  // Set the execution functions for both tasks
  t1->SetExecutionFunction(&task1_run);
  t2->SetExecutionFunction(&task2_run);
  
  // Begin execution
  failed += cs->Execute(argc,argv);

  test("Number failed",0,failed);

  MPI_Finalize();
  return failed;
}
