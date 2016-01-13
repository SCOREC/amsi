#ifndef AMSI_COMMPATTERN_H_
#define AMSI_COMMPATTERN_H_
#include "Assemblable.h"
#include "amsiMetaConfig.h"
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
    enum Role
    {
      SENDER = 0,
      RECVER = 1
    };
    CommPattern(int s1, int s2);
    void getSentTo(int rank, std::vector<int>& send_to) const;
    void getRecvedFrom(int rank, std::vector<int>& recv_from) const;
    int& operator()(int,int);
    int operator()(int,int) const;
    int getNumSenders() {return s1;}
    int getNumRecvers() {return s2;}
    virtual int Assemble(MPI_Comm,int);
    virtual void Reconcile();
  protected:
    friend std::ostream& operator<<(std::ostream&,const CommPattern&);
    friend CommPattern* CommPattern_CreateInverted(const CommPattern*);

    // naive implementation (full matrix) takes way more space than necessary
    int s1,s2;
    int * pattern;
  private:
    CommPattern();
  };

  /// Associated commpattern functions

    /// Output operator, used to print a matrix repressentation of the CommPattern object using the standard << operator
    std::ostream& operator<<(std::ostream& os, const CommPattern& obj);

    /// Create a new CommPattern which is an inversion of the old one (ie all sending processes in the old CommPattern are recvers in the produced CommPattern and vice-versa, essentially a matrix transposition)
    CommPattern * CommPattern_CreateInverted(const CommPattern * pattern);

    // retrieve information about the rank and remote index of a piece of local data by index
    std::pair<int,int> coupledInfoByIndex(CommPattern * cp,
					  CommPattern::Role send_recv,
					  int rank,
					  int index);



} // namespace amsi


#endif
