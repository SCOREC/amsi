#ifndef AMSI_COUPLING_DATA_H_
#define AMSI_COUPLING_DATA_H_
#include "amsiMPI.h"
#include "amsiObserver.h"
#include "amsiPArray.h"
namespace amsi
{
  class MultiscaleTerms : public PArrayI, public Observer
  {
  private:
    Scale * scl;
    MPI_Datatype dt;
    size_t bt_sz;
  public:
    MultiscaleTerms(Scale * s)
      : PArrayI(s->getComm())
      , Observer()
      , scl(s)
    {
      s->addObserver(this);
    }
    Scale * getScale() {return scl;}
    void setDataType(MPI_Datatype d)
    {
      dt = d;
      bt_sz = 1;
    }
    void setByteSize(size_t sz)
    {
      dt = MPI_BYTE;
      bt_sz = sz;
    }
    void onNotify()(Subject * s, Event e)
    {
      if(e = SYNCHRONIZE)
      {
        cm = ((Scale*)s)->getComm();
        synchronize();
      }
    }
  };
}
#endif
