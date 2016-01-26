#ifndef AMSI_MPI_H_
#define AMSI_MPI_H_
#include "amsiMPI.h"
#include "amsiUUID.h"
#include <hwloc.h>
#include <mpi.h>
#include <vector>
namespace amsi
{
  extern hwloc_topology_t machine_topology;
  /// brief : get a uuid associated with the hardware processor associated with the local mpi rank
  uuid getNodeID();
  // brief : get all mpi ranks in comm associated with the hardware processor p_id
  class RankSet;
  // todo (m) : consider moving MPI_Comm to the last parameter and having a default value of AMSI_COMM_WORLD
  void getRanksOnSocket(MPI_Comm c, uuid p_id, RankSet & rnks);
  void getRanksOnNode(MPI_Comm c, uuid n_id, RankSet & ranks);
  void getNodeSet(MPI_Comm c, std::vector<uuid> & nodes);
  void getNthRankOnNode(uuid n_id, rank_t nth, rank_t rnk, MPI_Comm = AMSI_COMM_WORLD);
  void getNthRankOnNodes(const std::vector<uuid> & nids, rank_t nth, RankSet * rnks, MPI_Comm = AMSI_COMM_WORLD);
}
#endif
