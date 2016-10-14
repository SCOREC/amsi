#ifndef AMSI_COMMPATTERN_H_
#define AMSI_COMMPATTERN_H_
#include "Assemblable.h"
#include "amsiMetaConfig.h"
#include "CommunicationManager.h"
#include <mpi.h>
#include <PCU.h>
#include <ostream>
#include <vector>
namespace amsi
{
  class CommPattern : public Assemblable
  {
  public:
    CommPattern(int so, int st)
      : s1(so)
      , s2(st)
    {}
    virtual int& operator()(int,int) = 0;
    virtual int operator()(int,int) const = 0;
    int getNumSenders() const {return s1;}
    int getNumRecvers() const {return s2;}
  protected:
    int s1;
    int s2;
  };
  class FullCommPattern : public CommPattern
  {
  public:
    FullCommPattern(int s1, int s2);
    virtual int& operator()(int,int);
    virtual int operator()(int,int) const;
    virtual void Assemble(MPI_Comm);
  private:
    FullCommPattern();
    int * pattern;
  };
  /// count the number of processes the specified recving process is being sent to
  //  @param rnk The rank in the recving task
  int countRanksSentTo(const CommPattern * cp, int rnk);
  /// count the number of processes the specified recving process is recving from
  int countRanksSentFrom(const CommPattern * cp, int rnk);
  /// get the task ranks of the processes the specified sending process is sending to
  void getRanksSentTo(const CommPattern * cp, int rnk, int * rnks);
  /// get the task ranks of the processes the specified recving process is recving from
  void getRanksSentFrom(const CommPattern * cp, int rnk, int * rnks);
  /// get the unit count for the specified rank to send to the ranks given by getRanksSentTo()
  void getUnitsSentTo(const CommPattern * cp, int rnk, int * sent_to);
  /// get the unit count for the specified rank to recv from the ranks given by getRanksRecvFrom()
  void getUnitsSentFrom(const CommPattern * cp, int rnk, int * recv_from);
  /// get the unit count sums for each rnk the sending task
  // essentially reconstruct the data dist the pattern is based on
  void getUnitsSent(const CommPattern * cp, int * snd);
  /// get the unit count sums for each rnk in the recving task
  // construct the recving data dist the pattern is based on
  void getUnitsRecv(const CommPattern * cp, int * recv);
  // TODO: pull these two functions into the CommPattern class,
  //  as their implementation should vary with different implementations
  //  of CommPatterns.
  void zeroCommPattern(CommPattern * cp);
  void zeroNonLocal(CommPattern * cp, int rnk, Role rl);
  std::ostream& operator<<(std::ostream& os, const CommPattern& obj);
  /// Create a new CommPattern which is an inversion of the old one (ie all sending processes in the old CommPattern are recvers in the produced CommPattern and vice-versa, essentially a matrix transposition)
  CommPattern * CommPattern_CreateInverted(const CommPattern * pattern);
  // retrieve information about the rank and remote index of a piece of local data by index
  std::pair<int,int> coupledInfoByIndex(CommPattern * cp,
                                        Role role,
                                        int rank,
                                        int index);
} // namespace amsi
#endif
