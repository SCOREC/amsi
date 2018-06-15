#ifndef AMSI_MPI_H_
#define AMSI_MPI_H_
#include "amsiMPI.h"
#include "amsiUUID.h"
//#include <hwloc.h>
#include <mpi.h>
#include <vector>
namespace amsi
{
  //extern hwloc_topology_t machine_topology;
  //class RankSet;
  /**
   * Get a uuid (misnomer) which identifies the hardware processor
   *  the local mpi rank is executing on. This is based on hashing
   *  MPI_Get_processor_name which by the standard gives a unique
   *  identifying string, so barring a hash conflict this should be
   *  fine.
   * @todo Should this be getSocketID?
   */
  //uuid getNodeID();
  /**
   * Get a vector containing the uuid of every hardware processor
   *  in the specified comm in no particular order.
   * @param nodes A vector containing the set of unique hardware processors
   *              belonging to the specified comm. Should be empty when passed.
   * @param c The mpi comm over which to perform the operation
   * @note This call is collective over the specified comm.
   * @todo Should this be getSocketSet?
   */
  //void getNodeSet(std::vector<uuid> & nodes, MPI_Comm c = AMSI_COMM_WORLD);
  // brief : get all mpi ranks in comm associated with the hardware processor p_id
  /**
   * Get the mpi ranks of the processes executing on the specific
   * hardware processor specified.
   * @note Currently unimplemented
   * @todo implement!
   */
  //void getRanksOnSocket(uuid p_id, RankSet & rnks, MPI_Comm c = AMSI_COMM_WORLD);
  /**
   * Get the mpi ranks (in the specified comm) of the processes executing
   *  on the specific machine node specified.
   * @param n_id The id of the node to retrieve the ranks executing on.
   * @param ranks The set of ranks on the specified machine node.
   * @param c The mpi comm in which the ranks are specified.
   */
  //void getRanksOnNode(uuid n_id, RankSet & ranks, MPI_Comm c = AMSI_COMM_WORLD);
  /**
   * Get the nth rank on the specified machine node in the specified comm.
   * @return The mpi rank of the nth process on machine node n_id
   */
  //int getNthRankOnNode(uuid n_id,
  //                     int nth,
  //                     MPI_Comm c = AMSI_COMM_WORLD);
  /**
   * Get the nth rank of every machine node specified in the specified comm.
   * @param nids The machine nodes to get the mpi rank of the nth local process on.
   * @param nth The local machine node process to retrieve the mpi rank of.
   * @param rnks The resulting set of mpi ranks.
   * @param c The MPI_Comm the resulting ranks are valid in.
   */
  //void getNthRankOnNodes(const std::vector<uuid> & nids,
  //                       int nth,
  //                       RankSet * rnks,
  //                       MPI_Comm c = AMSI_COMM_WORLD);
}
#endif
