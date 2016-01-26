#ifndef AMSI_PARRAY_H_
#define AMSI_PARRAY_H_
#include "amsiDistributed.h"
#include "amsiFail.h"
#include "amsiMPI.h"
namespace amsi
{
  template <typename T>
    class PArray : public Distributed
  {
  protected:
    MPI_Comm cm;
    int sz;
    T * arr;
    T f;
  public:
    PArray(MPI_Comm comm)
      : Distributed(false)
      , cm(comm)
      , sz(0)
      , arr(NULL)
      , f()
    {
      if(cm != MPI_COMM_NULL)
      {
	int s = 0;
        MPI_Comm_size(comm,&s);
	sz = s;
        arr = new T[sz]();
      }
      f = fail<T>();
    }
    T operator[](size_t idx) const { return (idx >= sz || idx < 0) ? f : arr[idx]; }
    T & operator[](size_t idx) { setValid(false); return (idx >= sz || idx < 0) ? f : arr[idx]; }
    size_t size() const { return sz; }
    T * begin() {return &arr[0];}
    T * end() {return &arr[0]+sz;}
    void synchronize()
    {
      if(!isValid())
      {
	int rnk = -1;
	MPI_Comm_rank(cm,&rnk);
	T vl = arr[rnk];
	// todo (m) : convert to non-blocking
	MPI_Allgather(&vl,1,mpi_type<T>(),
		      &arr[0],1,mpi_type<T>(),
		      cm);
	setValid(true);
      }
    }
  };
  typedef PArray<int> PArrayI;
  typedef PArray<double> PArrayD;
  typedef PArray<bool> PArrayB;
}

#endif
