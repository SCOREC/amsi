#include "amsiLocality.h"
#include "amsiRankSet.h"
#include "amsiMPI.h"
#include <algorithm>
namespace amsi
{
  hwloc_topology_t machine_topology;
  uuid getNodeID()
  {
    uuid lid = 0;
    char node_nm[MPI_MAX_PROCESSOR_NAME] = {};
    int nm_len = 0;
    MPI_Get_processor_name(node_nm, &nm_len);
    //std::string nm_str(node_nm);
    lid = generateUUID(std::string(node_nm));
    return lid;
  }
  void getNodeSet(std::vector<uuid> & nds, MPI_Comm cm)
  {
    int sz = 0;
    MPI_Comm_size(cm,&sz);
    int rnk = -1;
    MPI_Comm_rank(cm,&rnk);
    nds.resize(sz);
    uuid n_id = getNodeID();
    nds[rnk] = n_id;
    MPI_Allgather(&n_id,1,mpi_type<uuid>(),
                  &nds[0],1,mpi_type<uuid>(),
                  cm);
    std::sort(nds.begin(), nds.end());
    std::vector<uuid>::iterator ne = std::unique(nds.begin(), nds.end());
    nds.erase(ne,nds.end());
  }
  void getRanksOnSocket(uuid n_id, RankSet & rnks, MPI_Comm cm)
  {
  }
  void getRanksOnNode(uuid n_id, RankSet & rnks, MPI_Comm cm)
  {
    // init
    int sz = 0;
    MPI_Comm_size(cm,&sz);
    int rnk = -1;
    MPI_Comm_rank(cm,&rnk);
    // retrieve
    uuid pids[sz];
    pids[rnk] = n_id;
    MPI_Allgather(&n_id,1,mpi_type<uuid>(),
                  &pids[0],1,mpi_type<uuid>(),
                  cm);
    // process
    uuid * it = &pids[0];
    while((it = std::find(it,pids+sz,n_id)) != pids+sz)
    {
      rnks.insert(std::distance(&pids[0],it));
      it++;
    }
  }
  int getNthRankOnNode(uuid nid, int nth, MPI_Comm cm)
  {
    DefaultRankSet rnks;
    getRanksOnNode(nid,rnks,cm);
    std::vector<int> on_nd;
    pushTo(&rnks,std::back_inserter(on_nd));
    assert((unsigned)nth < on_nd.size());
    return on_nd.at(nth);
  }
  void getNthRankOnNodes(const std::vector<uuid> & nids, int nth, RankSet * rnks, MPI_Comm cm)
  {
    assert(rnks);
    for(std::vector<uuid>::const_iterator nid = nids.begin(); nid != nids.end(); nid++)
      rnks->insert(getNthRankOnNode(*nid,nth,cm));
  }
}
