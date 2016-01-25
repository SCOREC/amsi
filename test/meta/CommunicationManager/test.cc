// in-build
#include "../../test.h"

// in-project
#include "TaskManager.h"
#include "CommunicationManager.h"

// standard
#include <iostream>
#include <utility>

int task1_run(int &, char **&, MPI_Comm)
{
  return 0;
}

using namespace amsi;

int main(int argc, char * argv[])
{
  int failed = 0;
  MPI_Init(&argc,&argv);

  int rank = -1;
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);

  int size;
  MPI_Comm_size(MPI_COMM_WORLD,&size);

  std::cout << "Initializing test object(s):" << std::endl;
/*
  // Create a task manager which uses the MPI_COMM_WORLD comm to create the global ProcessSet
  TaskManager * tm = new TaskManager(MPI_COMM_WORLD);
  
  // Create a task using id "macro" consisting of 8 processes
  Task * macro = tm->createTask("macro",8);
  
  // Create a task using id "micro" consisting of 24 processes
  Task * micro = tm->createTask("micro",24);

  // Get the unique id's for the "macro" and "micro" tasks
  size_t t_id1 = tm->getTaskID("macro");
  size_t t_id2 = tm->getTaskID("micro");

  // Create a CommunicationManager
  CommunicationManager * cm = new CommunicationManager();

  // Make sure the CommunicationManager constructed
  failed += test_neq("CommunicationManager()",static_cast<CommunicationManager*>(NULL),cm);

  // Define a CommRelation between macro and micro and save the id
  size_t r_id1 = cm->defineRelation(t_id1,t_id2);

  // Test that the CommRelation id is valid
  failed += test_neq(".CommRelation_Define()",static_cast<size_t>(0),r_id1);

  // Test that the CommRelation exists
  failed += test(".CommRelation_Exists()",true,cm->CommRelation_Exists(t_id1,t_id2));

  // Retrieve the "macro" task
  std::pair<size_t,size_t> t_ids = cm->Relation_GetTasks(r_id1);

  failed += test(".Relation_GetTasks()",t_id1,t_ids.first);
  failed += test(".Relation_GetTasks()",t_id2,t_ids.second);
*/
  test("Total Failed",0,failed);

  MPI_Finalize();
  return failed;
}
