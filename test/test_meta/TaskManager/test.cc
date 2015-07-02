// in-build
#include "../../test.h"

// in-project
#include "TaskManager.h"

// standard
#include <iostream>
#include <utility>

using namespace amsi;

/// @brief Simple task-executing placeholed function for testing
/// @param The number of command-line arguments 
/// @param The command-line arguments
/// @param The comm on which this task is operating
/// @return An integer, 0 if success, non-zero if fail
int task1_run(int &, char **&, MPI_Comm)
{
  int failed = 0;
  
  // if this is even run the test succeeds
  failed += test(".Execute()",true,true);
  return failed;
}

/// test entry point
int main(int argc, char * argv[])
{
  int failed = 0;
  MPI_Init(&argc,&argv);

  int rank = -1;
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);

  int size;
  MPI_Comm_size(MPI_COMM_WORLD,&size);

  std::cout << "Initializing test object(s):" << std::endl;

  // Create a taskmanger test object using the world comm as the primary parallel execution space
  TaskManager * tm = new TaskManager(MPI_COMM_WORLD);

  // Make sure the task manager was successfully constructed
  failed += test_neq("TaskManager()",
		     static_cast<void*>(NULL),
		     static_cast<void*>(tm));

  // Create a task using the task manager using 8 processes
  Task * t1 = NULL;
  t1 = tm->createTask("test_task",8);
 
  // Test whether the task manager successfully created the task
  failed += test_neq(".Task_Create()",static_cast<void*>(NULL),static_cast<void*>(t1));

  // Test whether the task manager successfully recongizes the newly-created task exists
  failed += test(".Task_Exists()",true,tm->Task_Exists("test_task"));

  // Test whether the task manager recongizes a non-existent task
  failed += test(".Task_Exists()",false,tm->Task_Exists("test_task_fail"));

  // Retrieve a pointer to the newly-created task
  Task * t2 = NULL;
  t2 = tm->getTask("test_task");
  
  // Test that the original pointer the the task and the retrieved one both point to the same object
  failed += test(".Task_Get()",t1,t2);

  // Retrieve the task id
  size_t t_id = tm->getTaskID("test_task");

  // Test that the task id is non-zero
  failed += test_neq(".Task_GetID()",static_cast<size_t>(0),t_id);

  // Retrieve the task id for a non-existent task
  t_id = tm->getTaskID("test_task_fail");

  failed += test(".Task_GetID()",static_cast<size_t>(0),t_id);

  Task * t3 = tm->getLocalTask();
  
  failed += test(".Task_GetLocal()",static_cast<void*>(NULL),static_cast<void*>(t3));

  t1->SetExecutionFunction(&task1_run);

  tm->Execute(argc,argv);

  t2 = tm->createTask("test_task_2",24);

  failed += test_neq(".Task_Create()",static_cast<Task*>(NULL),t2);

  test("Number Failed",0,failed);

  MPI_Finalize();
  return failed;
}
