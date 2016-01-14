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
  class CommPattern : public Assemblable, public Reconcilable
  {
  public:
    CommPattern(int so, int st)
      : s1(so)
      , s2(st)
    {}
    virtual int& operator()(int,int) = 0;
    virtual int operator()(int,int) const = 0;

    void getSentTo(int rank, std::vector<int>& sent_to) const;
    void getRecvedFrom(int rank, std::vector<int>& recv_from) const;

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
    virtual void Reconcile();
  private:
    FullCommPattern();
    int * pattern;
  };

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
