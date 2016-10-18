#ifndef AMSI_DISTRIBUTED_H_
#define AMSI_DISTRIBUTED_H_
#include "amsiMPI.h"
namespace amsi
{
  /**
   * Currently this seems more like assemblable...
   * Distributed should probably be limited to determining whether the
   *  MPI Comm the class operates on is valid and provide an interface
   *  to make the comm valid if it is not..
   */
  class Distributed
  {
  protected:
    MPI_Comm cm;
    bool vld;
  public:
     Distributed(MPI_Comm c = MPI_COMM_NULL, bool v = false)
      : cm(c)
      , vld(false)
    { }
    bool isLocallyValid() {return vld;}
    bool isValid()
    {
      bool vld = cm == MPI_COMM_NULL;
      if(!vld)
      {
        int all = 0;
        MPI_Allreduce(&vld,&all,1,MPI_INTEGER,MPI_SUM,cm);
        vld = all > 0;
      }
      return vld;
    }
    void setValid(bool v) { vld = v; }
    MPI_Comm getComm() { return cm; }
    virtual void synchronize() = 0;
  };
}
#endif
