// in-build
#include "../../test.h"

// in-project
#include "amsi.h"
#include "ControlService.h"

// standard
#include <iostream>
#include <utility>

using namespace amsi;

int task1_run(int &, char **&, MPI_Comm)
{
  int failed = 0;

  // Retrieve the ControlService singleton object
  ControlService * cs = ControlService::Instance();

  // Get the local rank and determine an amount of locally-owned data
  Task * local_task = amsi::getLocal();
  
  int local_rank = local_task->LocalRank();
  int local_data_count = (local_rank % 4) * 4;

  // Create a DataDistribution, set the local data, and assemble the DD on the local Task
  local_task->DataDist_Create("send_to_task2");
  local_task->DataDist_SetLocal("send_to_task2",local_data_count);
  local_task->DataDist_Assemble("send_to_task2");

  // Create a CommPattern for the "send_to_task2" DataDistribution
  size_t pattern_id = cs->CreateCommPattern("send_to_task2","task1","task2");

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

  // Create a placeholder communication pattern to reconcile into
  size_t recv_pattern_id = cs->RecvCommPattern("send_to_task1",
					       "task2",
					       "",
					       "task1");
  
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

  return failed;
}


int task2_run(int &, char **&, MPI_Comm)
{
  int failed = 0;

  // Retrieve the ControlService object
  ControlService * cs = ControlService::Instance();

  Task * local_task = amsi::getLocal();
  int local_rank = local_task->LocalRank();

  // Create a placeholder CommPattern to reconcile into
  size_t pattern_id = cs->RecvCommPattern("send_to_task2",
					  "task1",
					  "",
					  "task2");
  
  failed += test_neq(".RecvCommPattern",static_cast<size_t>(0),pattern_id);

  // Reconcile the CommPattern from task1 to this task
  cs->CommPattern_Reconcile(pattern_id);

  // Recv data from task1
  std::vector<double> data;
  cs->Communicate(pattern_id,data,MPI_DOUBLE);

  //invert the comm mapping for use as the commpattern for the t2->t1 relation
  local_task->DataDist_Create("send_to_task1");
  local_task->DataDist_SetLocal("send_to_task1",data.size());
  local_task->DataDist_Assemble("send_to_task1");

  // invert the communication pattern used to get in the t1->t2 "send_to_task2" relation
  size_t send_pattern_id = cs->CommPattern_UseInverted(pattern_id,"send_to_task1","task2","task1");

  // assemble the pieces of the inverted pattern so each process has the full pattern (currently required for reconciliation)
  cs->CommPattern_Assemble(send_pattern_id); // collective on the task
  cs->CommPattern_Reconcile(send_pattern_id); // collective on the relation closure
  cs->Communicate(send_pattern_id,data,MPI_DOUBLE); // collective on the relation closure

  return failed;
}

int main(int argc, char * argv[])
{
  int failed = 0;
  amsi::use_petsc = false;
  amsi::use_simmetrix = false;
  
  amsi::initializer = new amsi::amsiControlInit;
  amsi::amsiInit(argc,argv);

  Task * t1 = amsi::tm->getTask("task1");
  Task * t2 = amsi::tm->getTask("task2");

  ControlService * cs = ControlService::Instance();

  failed += test_neq("ControlService::Instance()",static_cast<ControlService*>(NULL),cs);

  // Set the execution functions for both tasks
  t1->SetExecutionFunction(&task1_run);
  t2->SetExecutionFunction(&task2_run);
  
  // Begin execution
  failed += cs->Execute(argc,argv);

  test("Number failed",0,failed);

  amsi::amsiFree();
  return failed;
}
