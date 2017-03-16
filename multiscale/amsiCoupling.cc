#include "amsiCoupling.h"
#include <cassert>
#include <cstring> // for memset
#include <iomanip>
#include <iostream>
#include <numeric>
namespace amsi
{
  std::pair<int,int> coupledInfoByIndex(CommPattern * cp,
                                        Role role,
                                        int rank,
                                        int index)
  {
    std::pair<int,int> result;
    int remainder = 0;
    int coupled_rank = -1;
    int num_recvers = cp->getNumRecvers();
    int num_senders = cp->getNumSenders();
    if(role == SENDER)
    {
      int count = 0;
      for(int ii = 0; ii < num_recvers; ii++)
      {
        int other_count = (*cp)(rank,ii);
        if(count + other_count >= index)
        {
          remainder = index - count;
          coupled_rank = ii;
          break;
        }
        else
          count += other_count;
      }
      count = 0;
      for(int ii = 0; ii < rank; ii++)
        count += (*cp)(ii,coupled_rank);
      result.first = coupled_rank;
      result.second = count + remainder;
    }
    else if(role == RECVER)
    {
      int count = 0;
      for(int ii = 0; ii < num_senders; ii++)
      {
        int other_count = (*cp)(ii,rank);
        if(count + other_count >= index)
        {
          remainder = index - count;
          coupled_rank = ii;
          break;
        }
        else
          count += other_count;
      }
      count = 0;
      for(int ii = 0; ii < rank; ii++)
        count += (*cp)(coupled_rank,ii);
      result.first = coupled_rank;
      result.second = count + remainder;
    }
    return result;
  }
  int countRanksSendingTo(const CommPattern * cp, int rnk)
  {
    int cnt = 0;
    for(int ii = 0; ii < cp->getNumSenders(); ii++)
      if((*cp)(ii,rnk) > 0)
        cnt++;
    return cnt;
  }
  int countRanksSentToFrom(const CommPattern * cp, int rnk)
  {
    int cnt = 0;
    for(int ii = 0; ii < cp->getNumRecvers(); ii++)
      if((*cp)(rnk,ii) > 0)
        cnt++;
    return cnt;
  }
  void getRanksSendingTo(const CommPattern * cp, int rnk, int * rnks)
  {
    int hd = 0;
    for(int ii = 0; ii < cp->getNumSenders(); ii++)
      if((*cp)(ii,rnk) > 0)
      {
        rnks[hd] = ii;
        hd++;
      }
  }
  void getRanksSentToFrom(const CommPattern * cp, int rnk, int * rnks)
  {
    int hd = 0;
    for(int ii = 0; ii < cp->getNumRecvers(); ii++)
      if((*cp)(rnk,ii) > 0)
      {
        rnks[hd] = ii;
        hd++;
      }
  }
  void getUnitsSendingTo(const CommPattern * cp, int rnk, int * sent_to)
  {
    int hd = 0;
    for(int ii = 0; ii < cp->getNumSenders(); ii++)
    {
      int unts = (*cp)(ii,rnk);
      if(unts > 0)
      {
        sent_to[hd] = (*cp)(ii,rnk);
        hd++;
      }
    }
  }
  void getUnitsSentToFrom(const CommPattern * cp, int rnk, int * recv_from)
  {
    int hd = 0;
    for(int ii = 0; ii < cp->getNumRecvers(); ii++)
    {
      int unts = (*cp)(rnk,ii);
      if(unts > 0)
      {
        recv_from[hd] = (*cp)(rnk,ii);
        hd++;
      }
    }
  }
  void getUnitsSent(const CommPattern * cp, int * snt)
  {
    for(int ii = 0; ii < cp->getNumSenders(); ii++)
    {
      snt[ii] = 0;
      for(int jj = 0; jj < cp->getNumRecvers(); jj++)
        snt[ii] += (*cp)(ii,jj);
    }
  }
  void getUnitsRecv(const CommPattern * cp, int * rcv)
  {
    for(int jj = 0; jj < cp->getNumRecvers(); jj++)
    {
      rcv[jj] = 0;
      for(int ii = 0; ii < cp->getNumSenders(); ii++)
        rcv[jj] += (*cp)(ii,jj);
    }
  }
  /// @brief Default constructor.
  /// @param s1_ The number of processes in the ProcessSet related to the sending task
  /// @param s2_ The number of processes in the ProcessSet related to the recving task
  FullCommPattern::FullCommPattern(int s1_, int s2_)
    : CommPattern(s1_,s2_)
    , pattern()
  {
    pattern = new int[s1 * s2];
    memset(&pattern[0],0, sizeof(int) * s1 * s2);
  }

  /// @brief Access/Modification operator, analogous to the .GetDataCount(int,int) member function
  int& FullCommPattern::operator()(int r1, int r2)
  {
    unassembled();
    assert(r1 < s1 && r1 >= 0);
    assert(r2 < s2 && r2 >= 0);
    return pattern[r1 * s2 + r2];
  }

  /// @brief Access operator, analogous to the .GetDataCount(int,int) member function
  int FullCommPattern::operator()(int r1, int r2) const
  {
    assert(r1 < s1 && r1 >= 0);
    assert(r2 < s2 && r2 >= 0);
    return pattern[r1 * s2 + r2];
  }

  /// @brief Assemble a CommPattern across a given MPI_Comm (typically only called by another AMSI object.
  /// @param MPI_Comm The comm across which to assemble the CommPattern (should be the MPI_Comm containing the
  ///                 ranks associated with the sending task.
  /// @param rank The task-rank of the local process.
  void FullCommPattern::Assemble(MPI_Comm cm)
  {
    int rnk = 0;
    MPI_Comm_rank(cm,&rnk);
    int sz = -1;
    MPI_Comm_size(cm,&sz);
    assert(sz != -1);
    this->zeroNonLocal(rnk,SENDER);
    int rnks_cnt = countRanksSentToFrom(this,rnk);
    std::vector<int> rnks_cnts(sz);
    rnks_cnts[rnk] = rnks_cnt;
    MPI_Allgather(&rnks_cnt,1,mpi_type(rnks_cnt),
                  &rnks_cnts[0],1,mpi_type(rnks_cnt),
                  cm);
    std::vector<int> rnks(rnks_cnt);
    std::vector<int> cnts(rnks_cnt);
    getRanksSentToFrom(this,rnk,&rnks[0]);
    getUnitsSentToFrom(this,rnk,&cnts[0]);
    int ttl_cnt = rnks_cnts[0];
    int offsets[sz];
    offsets[0] = 0;
    for(int ii = 1; ii < sz; ii++)
    {
      offsets[ii] = offsets[ii-1] + rnks_cnts[ii-1];
      ttl_cnt += rnks_cnts[ii];
    }
    int all_rnks[ttl_cnt];
    int all_cnts[ttl_cnt];
    MPI_Allgatherv(&rnks[0],rnks_cnt,mpi_type<int>(),
                   &all_rnks[0],&rnks_cnts[0],&offsets[0],mpi_type<int>(),
                   cm);
    MPI_Allgatherv(&cnts[0],rnks_cnt,mpi_type<int>(),
                   &all_cnts[0],&rnks_cnts[0],&offsets[0],mpi_type<int>(),
                   cm);
    for(int ii = 0; ii < sz; ii++)
    {
      int cnt = rnks_cnts[ii];
      for(int jj = 0; jj < cnt; jj++)
        (*this)(ii,all_rnks[jj+offsets[ii]]) = all_cnts[jj+offsets[ii]];
    }
  }
  void FullCommPattern::zero()
  {
    for(int ii = 0; ii < s1; ++ii)
      for(int jj = 0; jj < s2; ++jj)
        (*this)(ii,jj) = 0;
  }
  void FullCommPattern::zeroNonLocal(int rnk, Role rl)
  {
    if(rl == SENDER)
      for(int ii = 0; ii < getNumSenders() && ii != rnk; ii++)
        for(int jj = 0; jj < getNumRecvers(); jj++)
          (*this)(ii,jj) = 0;
    else if(rl == RECVER)
      for(int ii = 0; ii < getNumSenders(); ii++)
        for(int jj = 0; jj < getNumRecvers() && jj != rnk; jj++)
          (*this)(ii,jj) = 0;
  }
  CommPattern * invertCommPattern(const CommPattern * pattern)
  {
    CommPattern * result = NULL;
    int new_senders = pattern->getNumRecvers();
    int new_recvers = pattern->getNumSenders();
    result = new FullCommPattern(new_senders,new_recvers);
    for(int ii = 0; ii < new_recvers; ii++)
      for(int jj = 0; jj < new_senders; jj++)
        (*result)(jj,ii) = (*pattern)(ii,jj);
    return result;
  }
  std::ostream& operator<<(std::ostream& os, const CommPattern& obj)
  {
    os << "CommPattern: " << std::endl;
    for(int ii = 0; ii < obj.getNumSenders(); ii++)
    {
      for(int jj = 0; jj < obj.getNumRecvers(); jj++)
      {
        os << std::setw(10) << obj(ii,jj) << " ";
      }
      os << std::endl;
    }
    return os;
  }
}
