#include "test.h"
#include "amsiMultiscale.h"
#include <iterator>
#include <vector>
int main(int argc, char * argv[])
{
  using namespace amsi;
  initMultiscale(argc,argv, MPI_COMM_WORLD);
  int failed = 0;
  std::vector<uuid> nodes;
  getNodeSet(nodes,AMSI_COMM_WORLD);
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
  int nm_nds = nodes.size();
  if(assignedTo(&macro))
  {
    const MPI_Comm cm = macro.getComm();
    int sz = 0;
    MPI_Comm_size(cm,&sz);
    failed += test("MPI_Comm_size()",nm_nds,sz);
  }
  int gsz = 0;
  MPI_Comm_size(AMSI_COMM_WORLD,&gsz);
  if(assignedTo(&micro))
  {
    const MPI_Comm cm = micro.getComm();
    int sz = 0;
    MPI_Comm_size(cm,&sz);
    failed += test("MPI_Comm_size()",gsz,sz);
  }
  micro.resignRanks(&macro_ranks);
  micro.synchronize();
  if(assignedTo(&micro))
  {
    const MPI_Comm cm = micro.getComm();
    int sz = 0;
    MPI_Comm_size(cm,&sz);
    failed += test("MPI_Comm_size()",gsz-nm_nds,sz);
  }
  freeMultiscale();
  return failed;
}
