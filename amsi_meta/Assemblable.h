#ifndef ASSEMBLABLE_H_
#define ASSEMBLABLE_H_

#include <mpi.h>

namespace amsi {

   class Assemblable
   {
   public:
     virtual int Assemble(MPI_Comm,int) = 0;
     virtual bool isAssembled() const = 0;
   private:
     bool assembled;
   };

   class Reconcilable
   {
   public:
     virtual void Reconcile() = 0;
     virtual bool isReconciled() const = 0;
   private:
     bool reconciled;
   };

} // namespace amsi


#endif
