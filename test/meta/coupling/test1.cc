#include "test.h"
#include "amsiCoupling.h"
#include "amsiUtil.h"
#include <iterator>
int main(int argc, char * argv[])
{
  using namespace amsi;
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
  Coupling macro_micro(&macro,&micro);
  failed += test(".isValid()",false,macro_micro.isValid());
  macro_micro.synchronize();
  failed += test(".isValid()",true,macro_micro.isValid());
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
  micro.resignRanks(&macro_ranks);
  micro.synchronize();
  if(assignedTo(&micro))
  {
    const MPI_Comm cm = micro.getComm();
    rank_t sz = 0;
    MPI_Comm_size(cm,&sz);
    failed += test("MPI_Comm_size()",gsz-nm_nds,sz);
  }
  failed += test(".isValid()",false,macro_micro.isValid());
  macro_micro.synchronize();
  freeUtil();
  return failed;
}
