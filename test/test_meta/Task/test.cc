// in-build
#include "../../test.h"

// in-project
#include "Task.h"
#include "ProcessSet.h"

// standard
#include <iostream>
#include <utility>

using namespace amsi;

int task1(int&,char**&,MPI_Comm)
{
  return 0;
}

int main(int argc, char * argv[])
{
  MPI_Init(&argc,&argv);

  int rank = -1;
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);

  int size;
  MPI_Comm_size(MPI_COMM_WORLD,&size);

  int failed = 0;

  // task manager allocates some free processes 
  std::cout<<"Creating ProcessSet with 0,size where size = "<<size<<std::endl;
  ProcessSet * ps = static_cast<ProcessSet*>(new ProcessSet_T<std::pair<int, int> >(0,size));

  // create a task with the assigned processes
  Task * t1 = new Task(ps);

  failed += test_neq("Task()",static_cast<Task*>(NULL),t1);
  
  t1->DataDist_Create("t1_data");
  
  failed += test(".DataDist_Exists()",true,t1->DataDist_Exists("t1_data"));
  failed += test(".DataDist_Exists()",false,t1->DataDist_Exists("t1_data_fail"));
  
  size_t dd_id = t1->DataDist_GetID("t1_data");
  
  failed += test_neq(".DataDist_GetID()",static_cast<size_t>(0),dd_id);
  
  failed += test(".LocalRank()",rank,t1->LocalRank());

  failed += test(".LocalToGlobalRank()",size >= 4 ? 4 : -1,t1->LocalToGlobalRank(4));

  t1->SetExecutionFunction(&task1);

  failed += test(".Execute()",0,t1->Execute(argc,argv));

  t1->DataDist_SetLocal("t1_data",rank % 3);
 
  failed += test(".DataDist_GetLocal()",rank % 3,t1->DataDist_GetLocal(dd_id));

  t1->DataDist_Assemble("t1_data");  

  test("Failed",0,failed);

  MPI_Finalize();
  return failed;

}
