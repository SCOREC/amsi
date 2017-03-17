#include "test.h"
#include "amsiMultiscale.h"
int task1(int &, char **&, MPI_Comm)
{
  int fld = 0;
  size_t rid = amsi::getRelationID(amsi::getMultiscaleManager(),
                                   amsi::getScaleManager(),
                                   "task1","task2");
  amsi::ControlService * cs = amsi::ControlService::Instance();
  std::string tst_str("This is a test of the inter-scale broadcast system. This is only a test.");
  std::vector<MPI_Request> rqsts;
  cs->aSendBroadcast(std::back_inserter(rqsts),
                     rid,
                     tst_str.c_str(),
                     tst_str.size()+1); // need to account for null terminated c-string which adds a character
  MPI_Status sts[rqsts.size()];
  MPI_Waitall(rqsts.size(),&rqsts[0],&sts[0]);
  return fld;
}
int task2(int &, char **&, MPI_Comm)
{
  int fld = 0;
  size_t rid = amsi::getRelationID(amsi::getMultiscaleManager(),
                                   amsi::getScaleManager(),
                                   "task1","task2");
  amsi::ControlService * cs = amsi::ControlService::Instance();
  MPI_Request rqst;
  char * rcv_bfr = NULL;
  size_t sz = 0;
  while((sz = cs->aRecvBroadcastSize<char>(rid)) == 0)
  {
    // process other stuff
  }
  rcv_bfr = new char [sz];
  cs->aRecvBroadcast(&rqst,rid,rcv_bfr,sz);
  MPI_Status sts;
  MPI_Wait(&rqst,&sts);
  std::string tst_str(rcv_bfr);
  std::cout << tst_str << std::endl;
  return fld;
}
int main(int argc, char * argv[])
{
  int fld = 0;
  amsi::initMultiscale(argc,argv);
  amsi::Task * t1 = amsi::getScaleManager()->getTask("task1");
  amsi::Task * t2 = amsi::getScaleManager()->getTask("task2");
  amsi::ControlService * cs = amsi::ControlService::Instance();
  t1->setExecutionFunction(&task1);
  t2->setExecutionFunction(&task2);
  fld += cs->Execute(argc,argv);
  test("Tests failed",0,fld);
  amsi::freeMultiscale();
  return fld;
}
