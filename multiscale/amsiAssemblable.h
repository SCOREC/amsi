#ifndef AMSI_ASSEMBLABLE_H_
#define AMSI_ASSEMBLABLE_H_
#include <mpi.h>
namespace amsi
{
   class Assemblable
   {
   public:
     virtual void Assemble(MPI_Comm) = 0;
     bool isAssembled() {return assembled;}
   protected:
     void unassembled() {assembled = false;}
     bool assembled;
   };
   class Reconcilable
   {
   public:
     virtual void Reconcile(MPI_Comm) = 0;
     bool isReconciled() {return reconciled;}
   protected:
     void unreconciled() {reconciled = false;}
     bool reconciled;
   };
   void Assemble(Assemblable * a, MPI_Comm cm);
   /**
    * Get the number of peer-to-peer sends required from rank
    *  t1rnk to complete reconciliation of a piece of control
    *  data from a scale of size t1s to a scale of size t2s.
    */
   int getReconcileSendCount(int t1s, int t2s, int t1rnk);
   /**
    * Get an array of the foreign scale ranks required for
    *  completion of a reconciliation operation from rank t1rnk
    *  in a scale of size t1s to a scale of size t2s.
    *
    */
   void getReconcileSendRanks(int t1s, int t2s, int t1rnk, int * rnks);
   /**
    *
    * todo : incorporate into test case
    */
   int getNthReconcileSendRank(int t1s, int t2s, int t1rnk, int nth);
}
#endif
