#ifndef MPI_T_H_
#define MPI_T_H_
#include <mpi.h>
#include <cassert>
#include <cstring> //memcpy
#include <iostream>
#include <PCU.h>
[[deprecated(
    "use world communicator from MPI object")]] extern MPI_Comm AMSI_COMM_WORLD;
[[deprecated("use scale communicator from Multiscale object")]] extern MPI_Comm AMSI_COMM_SCALE;
namespace amsi {
  class MPIComm {
    public:
    MPIComm() : cm_(MPI_COMM_NULL), rank_(-1), size_(-1){};
    explicit MPIComm(MPI_Comm cm)
    {
      MPI_Comm_dup(cm, &cm_);
      MPI_Comm_rank(cm_, &rank_);
      MPI_Comm_size(cm_, &size_);
    }
    MPIComm(const MPIComm& other)
    {
      MPI_Comm_dup(other.cm_, &cm_);
      rank_ = other.rank_;
      size_ = other.size_;
    }
    [[nodiscard]] MPIComm(MPIComm&& other) noexcept
    {
      std::swap(cm_, other.cm_);
      std::swap(rank_, other.rank_);
      std::swap(size_, other.size_);
    }
    MPIComm& operator=(const MPIComm& other)
    {
      if (this != &other) {
        MPI_Comm_dup(other.cm_, &cm_);
        rank_ = other.rank_;
        size_ = other.size_;
      }
      return *this;
    }
    MPIComm& operator=(MPIComm&& other) noexcept
    {
      std::swap(cm_, other.cm_);
      std::swap(rank_, other.rank_);
      std::swap(size_, other.size_);
      return *this;
    }
    void set(MPI_Comm cm)
    {
      cm_ = cm;
      MPI_Comm_rank(cm, &rank_);
      MPI_Comm_size(cm, &size_);
    }
    ~MPIComm() = default;
    MPI_Comm cm_;
    int rank_;
    int size_;

    public:
    [[nodiscard]] int rank() const noexcept { return rank_; }
    [[nodiscard]] int size() const noexcept { return size_; }
    [[nodiscard]] MPI_Comm comm() const noexcept { return cm_; }
  };
  class MPI {
    private:
    class MPIScopeGuard {
      public:
      [[nodiscard]] MPIScopeGuard(int argc, char** argv)
      {
        int initialized;
        // should check the error here
        MPI_Initialized(&initialized);
        if (!initialized) {
          initialized_here_ = true;
          MPI_Init(&argc, &argv);
        }
      };
      MPIScopeGuard(const MPIScopeGuard&) = delete;
      MPIScopeGuard(MPIScopeGuard&& other) noexcept
      {
        std::swap(initialized_here_, other.initialized_here_);
      }
      MPIScopeGuard& operator=(const MPIScopeGuard& other) = delete;
      MPIScopeGuard& operator=(MPIScopeGuard&& other) noexcept
      {
        std::swap(initialized_here_, other.initialized_here_);
      }
      ~MPIScopeGuard()
      {
        if (initialized_here_) {
          int finalized;
          MPI_Finalized(&finalized);
          if (finalized) {
            std::cerr << "WARNING: MPI finalize called elsewhere despite this "
                         "scope guard being responsible for MPI finalization.";
          }
          MPI_Finalize();
        }
      }
      // need copy move assignment/constructor
      private:
      bool initialized_here_{false};
    };
    class PCUScopeGuard {
      public:
      [[nodiscard]] PCUScopeGuard()
      {
        if (!PCU_Comm_Initialized()) {
          initialized_here_ = true;
          PCU_Comm_Init();
        }
      }
      PCUScopeGuard(const PCUScopeGuard&) = delete;
      PCUScopeGuard(PCUScopeGuard&& other) noexcept
      {
        std::swap(initialized_here_, other.initialized_here_);
      }
      PCUScopeGuard& operator=(const PCUScopeGuard& other) = delete;
      PCUScopeGuard& operator=(PCUScopeGuard&& other) noexcept
      {
        std::swap(initialized_here_, other.initialized_here_);
      }
      ~PCUScopeGuard()
      {
        if (initialized_here_) {
          PCU_Comm_Free();
        }
      }

      private:
      bool initialized_here_{false};
    };

    public:
    // critical that environment_ is initialized first because that has the
    // call to MPI_Init
    MPI(int argc, char** argv, MPI_Comm cm = MPI_COMM_WORLD)
        : environment_{argc, argv}, self_{cm}, world_{cm}
    {
      AMSI_COMM_WORLD = world_.comm();
      // this seems to have been assumed to always exist in the analysis codes
      // even when multiscale run isn't happening. Therefore, we set to the
      // world communicator here. The multiscale class resets it to the sub
      // communicator. the AMSI_COMM variables are depreciated and eventually we
      // won't have this insanity
      AMSI_COMM_SCALE = world_.comm();
    }
    [[nodiscard]] const MPIComm& getSelf() const noexcept { return self_; }
    [[nodiscard]] const MPIComm& getWorld() const noexcept { return world_; }

    private:
    [[maybe_unused]] MPIScopeGuard environment_;
    [[maybe_unused]] PCUScopeGuard pcu_environment_;
    MPIComm self_;
    MPIComm world_;
  };
  template <typename T>
  MPI_Datatype mpi_type();
  template <typename T>
  MPI_Datatype mpi_type(T t);
  template <typename T>
  T comm_sum(T v, MPI_Comm cm = AMSI_COMM_SCALE)
  {
    T sm = 0;
    MPI_Datatype tp = mpi_type(v);
    size_t sz = 1;
    if (tp == MPI_BYTE) sz *= sizeof(T);
    MPI_Allreduce(&v, &sm, sz, mpi_type<T>(), MPI_SUM, cm);
    return sm;
  }
  template <typename T>
  T comm_scan(T v, MPI_Op op, MPI_Comm cm = AMSI_COMM_SCALE)
  {
    T scn = 0;
    MPI_Scan(&v,&scn,1,mpi_type<T>(),op,cm);
    return scn;
  }
  template <typename T>
    T comm_min(T v, MPI_Comm cm = AMSI_COMM_SCALE)
  {
    T mn = 0;
    MPI_Allreduce(&v,&mn,1,mpi_type<T>(),MPI_MIN,cm);
    return mn;
  }
  template <typename T>
    T comm_max(T v, MPI_Comm cm = AMSI_COMM_SCALE)
  {
    T mx = 0;
    MPI_Allreduce(&v,&mx,1,mpi_type<T>(),MPI_MAX,cm);
    return mx;
  }
  template <typename T>
    T comm_avg(T v, int c, MPI_Comm cm = AMSI_COMM_SCALE)
  {
    T vl = comm_sum(v,cm);
    int cnt = comm_sum(c,cm);
    assert(cnt != 0);
    return vl / cnt;
  }
  template <typename T>
    void send(T * msg, int to, int cnt = 1, MPI_Comm cm = AMSI_COMM_SCALE)
  {
    MPI_Send(msg,cnt,mpi_type<T>(),to,0,cm);
  }
  template <typename T>
    MPI_Request asend(T * msg, int to, int cnt = 1, MPI_Comm cm = AMSI_COMM_SCALE)
  {
    MPI_Request rqst;
    MPI_Isend((void*)msg,cnt,mpi_type<T>(),to,0,cm,&rqst);
    return rqst;
  }
  template <typename T>
    MPI_Status recv(T * msg, int frm = MPI_ANY_SOURCE, int cnt = 1, MPI_Comm cm = AMSI_COMM_SCALE)
  {
    MPI_Status s;
    MPI_Recv(msg,cnt,mpi_type<T>(),frm,MPI_ANY_TAG,cm,&s);
    return s;
  }
  // test if there is a message from rank frm waiting to be recv'd return a sz > 0 if so
  template <typename T>
    int arecv_sz(int & frm, MPI_Comm cm = AMSI_COMM_SCALE)
  {
    int sz = 0;
    MPI_Status s;
    int flg = 0;
    MPI_Iprobe(frm,0,cm,&flg,&s);
    if(flg)
    {
      MPI_Get_count(&s,mpi_type<T>(),&sz);
      frm = s.MPI_SOURCE;
    }
    return sz;
  }
  // asynchronously recv a message from frm with cnt elements
  template <typename T>
    MPI_Request arecv(T * msg, int frm = MPI_ANY_SOURCE, int cnt = 1, MPI_Comm cm = AMSI_COMM_SCALE)
  {
    MPI_Request rqst;
    MPI_Irecv(msg,
              cnt,
              mpi_type<T>(),
              frm,
              MPI_ANY_TAG,
              cm,
              &rqst);
    return rqst;
  }
}
#include "amsiMPI_impl.h"
#endif
