#include "test.h"
#include "amsiMultiscale.h"
#include "amsiControlService.h"
#include <iostream>
#include <utility>
using namespace amsi;
int task1_run(int &, char **&, MPI_Comm)
{
  int failed = 0;
  ControlService * cs = ControlService::Instance();
  Task * local_task = amsi::getLocal();
  int local_rank = local_task->localRank();
  int local_data_count = (local_rank % 4) * 4;
  local_task->createDD("send_to_task2");
  local_task->setLocalDDValue("send_to_task2",local_data_count);
  local_task->assembleDD("send_to_task2");
  size_t pattern_id = cs->CreateCommPattern("send_to_task2","task1","task2");
  failed += test_neq(".CreateCommPattern()",static_cast<size_t>(0),pattern_id);
  cs->CommPattern_Reconcile(pattern_id);
  std::vector<double> data(local_data_count);
  for(int ii = 0; ii < local_data_count; ii++)
    data[ii] =  static_cast<double>(local_data_count * ii) / static_cast<double>(local_rank);
  cs->Communicate(pattern_id,data,MPI_DOUBLE);
  size_t recv_pattern_id = cs->RecvCommPattern("send_to_task1",
                                               "task2",
                                               "",
                                               "task1");
  cs->CommPattern_Reconcile(recv_pattern_id);
  std::vector<double> result_data;
  cs->Communicate(recv_pattern_id,result_data,MPI_DOUBLE);
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
  ControlService * cs = ControlService::Instance();
  Task * local_task = amsi::getLocal();
  size_t pattern_id = cs->RecvCommPattern("send_to_task2",
                                          "task1",
                                          "",
                                          "task2");
  failed += test_neq(".RecvCommPattern",static_cast<size_t>(0),pattern_id);
  cs->CommPattern_Reconcile(pattern_id);
  std::vector<double> data;
  cs->Communicate(pattern_id,data,MPI_DOUBLE);
  local_task->createDD("send_to_task1");
  local_task->setLocalDDValue("send_to_task1",data.size());
  local_task->assembleDD("send_to_task1");
  size_t send_pattern_id = cs->CommPattern_UseInverted(pattern_id,"send_to_task1","task2","task1");
  cs->CommPattern_Assemble(send_pattern_id); // collective on the task
  cs->CommPattern_Reconcile(send_pattern_id); // collective on the relation closure
  cs->Communicate(send_pattern_id,data,MPI_DOUBLE); // collective on the relation closure
  return failed;
}
int main(int argc, char * argv[])
{
  int failed = 0;
  amsi::initMultiscale(argc,argv);
  Task * t1 = amsi::tm->getTask("task1");
  Task * t2 = amsi::tm->getTask("task2");
  ControlService * cs = ControlService::Instance();
  cs->setSuppressOutput(false);
  failed += test_neq("ControlService::Instance()",static_cast<ControlService*>(NULL),cs);
  t1->setExecutionFunction(&task1_run);
  t2->setExecutionFunction(&task2_run);
  failed += cs->Execute(argc,argv);
  test("Number failed",0,failed);
  amsi::freeMultiscale();
  return failed;
}
