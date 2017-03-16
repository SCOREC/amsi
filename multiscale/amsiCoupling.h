#ifndef AMSI_COMMPATTERN_H_
#define AMSI_COMMPATTERN_H_
#include "amsiAssemblable.h"
#include "amsiMultiscaleConfig.h"
#include "amsiCommunicationManager.h"
#include <mpi.h>
#include <PCU.h>
#include <ostream>
#include <vector>
namespace amsi
{
  class CommPattern : public Assemblable
  {
  protected:
    int s1;
    int s2;
    void zeroNonLocal(int,Role);
  public:
    CommPattern(int so, int st)
      : s1(so)
      , s2(st)
    {}
    virtual int& operator()(int,int) = 0;
    virtual int operator()(int,int) const = 0;
    virtual void zero() = 0;
    int getNumSenders() const {return s1;}
    int getNumRecvers() const {return s2;}
  };
  class FullCommPattern : public CommPattern
  {
  public:
    FullCommPattern(int s1, int s2);
    virtual int& operator()(int,int);
    virtual int operator()(int,int) const;
    virtual void Assemble(MPI_Comm);
    virtual void zero();
  private:
    FullCommPattern();
    int * pattern;
  protected:
    void zeroNonLocal(int,Role);
  };
  /**
   * Count the processes in the sending scale which will send data to the specified recving process.
   */
  int countRanksSendingTo(const CommPattern * cp, int rnk);
  /**
   * Count the processes in the recving scale which will recv data from the specified sending process.
   */
  int countRanksSentToFrom(const CommPattern * cp, int rnk);
  /**
   * Get the scale ranks of the processes in the sending scale the specified recving process is recving from.
   */
  void getRanksSendingTo(const CommPattern * cp, int rnk, int * rnks);
  /**
   * Get the scale ranks of the processes in the recving scale the specified sending process is sending to.
   */
  void getRanksSentToFrom(const CommPattern * cp, int rnk, int * rnks);
  /**
   * Get the unit counts for each process in the sending scale sending data to the specified recving process.
   */
  void getUnitsSendingTo(const CommPattern * cp, int rnk, int * sent_to);
  /**
   * Get the unit counts for each process in the recving scale recving data from the specified sending process.
   */
  void getUnitsSentToFrom(const CommPattern * cp, int rnk, int * recv_from);
  /**
   * Get the unit count sums for each process in the sending scale, this essentially reconstructs the DataDistribution.
   */
  void getUnitsSent(const CommPattern * cp, int * snd);
  /**
   * Get the unit count sums for each process in the recving scales, this essentially reconstructs the induced DataDistribution.
   */
  void getUnitsRecv(const CommPattern * cp, int * recv);
  /**
   * Create a new CommPattern by inverting the old one. (Essentially a matrix transposition).
   */
  CommPattern * invertCommPattern(const CommPattern * pattern);
  /**
   * Retrieve information about the rank and remote index of a piece of scale local data by index.
   */
  std::pair<int,int> coupledInfoByIndex(CommPattern * cp, Role role, int rank, int index);
  /**
   * Print a CommPattern to stream.
   */
  std::ostream& operator<<(std::ostream& os, const CommPattern& obj);
}
#endif
