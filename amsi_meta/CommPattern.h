#ifndef COMMPATTERN_H_
#define COMMPATTERN_H_

#include "Assemblable.h"
#include "amsiMetaConfig.h"

#include <mpi.h>

#ifdef CORE
#include <PCU.h>
#endif

#include <ostream>
#include <vector>

namespace amsi {

    /// Decleration of the CommPattern class and interface. Used to coordinate send/recv communication operations between active task-groups.
    class CommPattern : public Assemblable, public Reconcilable
    {
    public:
      CommPattern(int s1, int s2);

      int GetDataCount(int rank1, int rank2) const;
      void GetSendTo(int rank, std::vector<int>& send_to) const;
      void GetRecvFrom(int rank, std::vector<int>& recv_from) const;

      int& operator()(int,int);
      int operator()(int,int) const;

      // Assemblable interface
      int Assemble(MPI_Comm,int);
      bool isAssembled() const {return assembled;}

      // Reconcilable interface
      void Reconcile();
      bool isReconciled() const { return reconciled; }

      int * getPattern() {return pattern;}
      int getSendTaskSize() {return s1;}
      int getRecvTaskSize() {return s2;}

    protected:
      friend std::ostream& operator<<(std::ostream&,const CommPattern&);
      friend CommPattern* CommPattern_CreateInverted(const CommPattern*);

      // naive implementation (full matrix) takes way more space than necessary
      int s1,s2;
      int * pattern;
    private:
      
      bool assembled;
      bool reconciled;
    };

    /// Associated commpattern functions

    /// Output operator, used to print a matrix repressentation of the CommPattern object using the standard << operator
    std::ostream& operator<<(std::ostream& os, const CommPattern& obj);

    /// Create a new CommPattern which is an inversion of the old one (ie all sending processes in the old CommPattern are recvers in the produced CommPattern and vice-versa, essentially a matrix transposition)
    CommPattern * CommPattern_CreateInverted(const CommPattern * pattern);

} // namespace amsi


#endif
