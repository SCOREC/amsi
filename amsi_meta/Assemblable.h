#ifndef ASSEMBLABLE_H_
#define ASSEMBLABLE_H_

#include <mpi.h>

namespace amsi {

   class Assemblable
   {
   public:
     virtual int Assemble(MPI_Comm,int) = 0;
     bool isAssembled() {return assembled;}
   protected:
     void unassembled() {assembled = false;}
     bool assembled;
   };

   class Reconcilable
   {
   public:
     virtual void Reconcile() = 0;
     bool isReconciled() {return reconciled;}
   protected:
     void unreconciled() {reconciled = false;}
     bool reconciled;
   };

} // namespace amsi


#endif
