#include "test.h"
#include "amsiOps.h"
#include "amsiCoupling.h"
#include "amsiPArray.h"
#include "amsiUtil.h"
#include <functional>
#include <iterator>
using namespace amsi;
Coupling * macro_micro;
int * macro_buffer;
int * micro_buffer;
uuid macro_send(Scale * macro)
{
  return NULL;
  //return multiscaleSend(macro_micro,pln,(char*)macro_buffer,mpi_type<int>());
}
uuid micro_recv(Scale * micro)
{
  return NULL;
  //CouplingPlan * pln = NULL;
  //return multiscaleRecv(macro_micro,pln,(char*)micro_buffer,mpi_type<int>());
}
int main(int argc, char * argv[])
{
  initUtil(argc,argv);
  int failed = 0;
  std::vector<uuid> nodes;
  getNodeSet(AMSI_COMM_WORLD,nodes);
  DefaultRankSet macro_ranks;
  getNthRankOnNodes(nodes,0,&macro_ranks);
  Scale macro;
  macro.assignRanks(&macro_ranks);
  DefaultRankSet micro_ranks;
  getGlobalRanks(&micro_ranks);
  Scale micro;
  micro.assignRanks(&micro_ranks);
  macro.synchronize();
  micro.synchronize();
  macro_micro = new Coupling(&macro,&micro);
  failed += test(".isValid()",false,macro_micro->isValid());
  macro_micro->synchronize();
  failed += test(".isValid()",true,macro_micro->isValid());
  int nm_nds = nodes.size();
  if(assignedTo(&macro))
  {
    const MPI_Comm cm = macro.getComm();
    rank_t sz = 0;
    MPI_Comm_size(cm,&sz);
    failed += test("MPI_Comm_size()",nm_nds,sz);
  }
  rank_t gsz = 0;
  MPI_Comm_size(AMSI_COMM_WORLD,&gsz);
  if(assignedTo(&micro))
  {
    const MPI_Comm cm = micro.getComm();
    rank_t sz = 0;
    MPI_Comm_size(cm,&sz);
    failed += test("MPI_Comm_size()",gsz,sz);
  }
  CouplingPlan * pln = createCouplingPlan(macro_micro);
  if(assignedTo(&macro))
  {
    assert(macro.isValid());
    const MPI_Comm cm = macro.getComm();
    int sz = 0;
    MPI_Comm_size(cm,&sz);
    int rnk = -1;
    MPI_Comm_rank(cm,&rnk);
    PArrayI dd(AMSI_COMM_LOCAL);
    dd[rnk] = rnk;
    dd.synchronize();
    int ttl = 0;
    for(int ii = 0; ii < sz; ii++)
      ttl += dd[ii];
    int buf[rnk];
    for(int ii = 0; ii < rnk; ii++)
      buf[rnk] = ii;
    //write_send(pln,buf,rnk*sizeof(int));
  }
  /*
  if(assignedTo(&micro))
  {
    assert(micro.isValid());
    int * buf = NULL;
    size_t sz = 0;
    read_recv(pln,&buf,sz);
  }
  while(!checkOperation(micro_rcv_id))
  {
    
  }
  */
  freeUtil();
  return failed;
}
