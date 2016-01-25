#ifndef AMSI_SCALE_H_
#define AMSI_SCALE_H_
#include "amsiMetaConfig.h"
#include <amsiDistributed.h>
#include <amsiHost.h>
#include <amsiMPI.h>
#include <amsiRankSet.h>
#include <amsiObserver.h>
#include <map>
#include <string>
namespace amsi
{
  // responsible for maintaining the MPI_Comm used for scale-collective operations... thats it, other scale-related functionality should go elsewhere
  class Scale : public Subject, public Distributed
  {
  protected:
    RankSet * rnks;
    MPI_Comm comm;
    void reinitComm()
    {
      if(!isValid())
      {
        if(comm && comm != MPI_COMM_NULL)
          MPI_Comm_free(&comm);
        comm = makeComm(rnks);
        setValid(true);
        notify(SYNCHRONIZE);
      }
    }
  public:
    Scale()
      : Subject()
      , Distributed(false)
      , rnks(new DefaultRankSet)
      , comm()
    { }
    void assignRanks(RankSet * ps, MPI_Comm cm = AMSI_COMM_WORLD);
    // possibly make validResign a protected member, and change resignRanks
    //  to return bool if the resign cannot/did not succeed
    bool validResign(RankSet * ps, MPI_Comm cm = AMSI_COMM_WORLD);
    void resignRanks(RankSet * ps, MPI_Comm cm = AMSI_COMM_WORLD);
    // collective on current rank set in amsi comm world
    void synchronize() { reinitComm(); }
    const RankSet * getRankSet() const {return rnks;}
    const MPI_Comm getComm()
    {
      AMSI_DEBUG
      (
        if(!isValid())
          std::cerr << "WARNING: retrieving communicator "
                    << "from invalid Scale!" << std::endl;
      )
      return comm;
    }
  };
  /**
   * Determine whether the local process is assigned to the supplied Scale.
   * @param sc The Scale to check assignment to
   * @return Whether the local process is assigned to the Scale
   * @note If the Scale is invalid the local process may be assigned to the Scale
   *        but not belong to the MPI_Comm managed by the Scale. Prior to retrieving
   *        the managed MPI_Comm from the Scale a call to .isValid() should be made
   *        to determine the validity of the Scale, followed by a call to
   *        .synchronize() in order to make the Scale valid (if necessary).
   */
  bool assignedTo(Scale * sc);
  /**
   * Convenience function to get the size of the current rank set assigned to
   *  the provided scale.
   * @param sc The scale to retrieve size information from
   * @return The number of MPI ranks in the RankSet assigned to sc
   */
  inline int getSize(Scale * sc) { return sc->getRankSet()->size(); }
} // namespace amsi

#endif
