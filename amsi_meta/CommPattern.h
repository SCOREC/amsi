#ifndef AMSI_COMMPATTERN_H_
#define AMSI_COMMPATTERN_H_
#include "Assemblable.h"
#include "amsiMetaConfig.h"
#include "CommunicationManager.h"
#include <mpi.h>
#ifdef CORE
#include <PCU.h>
#endif
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
    virtual int Assemble(MPI_Comm);
  private:
    FullCommPattern();
    int * pattern;
  };
  /// count the number of processes the specified sending process is sending to
  int countRanksSentTo(const CommPattern * cp, int rnk);
  /// count the number of processes the specified recving process is recving from
  int countRanksRecvFrom(const CommPattern * cp, int rnk);
  /// get the task ranks of the processes the specified sending process is sending to
  void getRanksSentTo(const CommPattern * cp, int rnk, int * rnks);
  /// get the task ranks of the processes the specified recving process is recving from
  void getRanksRecvFrom(const CommPattern * cp, int rnk, int * rnks);
  /// get the unit count for the specified rank to send to the ranks given by getRanksSentTo()
  void getUnitsSentTo(const CommPattern * cp, int rnk, int * sent_to);
  /// get the unit count for the specified rank to recv from the ranks given by getRanksRecvFrom()
  void getUnitsRecvFrom(const CommPattern * cp, int rnk, int * recv_from);;
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
