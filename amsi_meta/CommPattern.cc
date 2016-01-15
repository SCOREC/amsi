#include "CommPattern.h"
#include <cassert>
#include <cstring> // for memset
#include <iomanip>
#include <iostream>
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
  int countRanksSentTo(const CommPattern * cp, int rnk)
  {
    int cnt = 0;
    for(int ii = 0; ii < cp->getNumSenders(); ii++)
      if((*cp)(rnk,ii) > 0)
        cnt++;
    return cnt;
  }
  int countRanksRecvFrom(const CommPattern * cp, int rnk)
  {
    int cnt = 0;
    for(int ii = 0; ii < cp->getNumRecvers(); ii++)
      if((*cp)(ii,rnk) > 0)
        cnt++;
    return cnt;
  }
  void getRanksSentTo(const CommPattern * cp, int rnk, int * rnks)
  {
    int hd = 0;
    for(int ii = 0; ii < cp->getNumSenders(); ii++)
      if((*cp)(rnk,ii) > 0)
      {
        rnks[hd] = ii;
        hd++;
      }
  }
  void getRanksRecvFrom(const CommPattern * cp, int rnk, int * rnks)
  {
    int hd = 0;
    for(int ii = 0; ii < cp->getNumRecvers(); ii++)
      if((*cp)(ii,rnk) > 0)
      {
        rnks[hd] = ii;
        hd++;
      }
  }
  void getUnitsSentTo(const CommPattern * cp, int rnk, int * sent_to)
  {
    for(int ii = 0; ii < cp->getNumSenders(); ii++)
      sent_to[ii] = (*cp)(rnk,ii);
  }
  void getUnitsRecvFrom(const CommPattern * cp, int rnk, int * recv_from)
  {
    for(int ii = 0; ii < cp->getNumRecvers(); ii++)
      recv_from[ii] = (*cp)(ii,rnk);
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
    return pattern[r1 * s2 + r2];
  }

  /// @brief Access operator, analogous to the .GetDataCount(int,int) member function
  int FullCommPattern::operator()(int r1, int r2) const
  {
    return pattern[r1 * s2 + r2];
  }

  /// @brief Assemble a CommPattern across a given MPI_Comm (typically only called by another AMSI object.
  /// @param MPI_Comm The comm across which to assemble the CommPattern (should be the MPI_Comm containing the
  ///                 ranks associated with the sending task.
  /// @param rank The task-rank of the local process.
  int FullCommPattern::Assemble(MPI_Comm cm)
  {
    int result = 0;
    if(!assembled)
    {
      int rnk = 0;
      MPI_Comm_rank(cm,&rnk);
      int sz = -1;
      MPI_Comm_size(cm,&sz);
      assert(sz != -1);
      int rnks_cnt = countRanksSentTo(this,rnk);
      std::vector<int> snd_to(rnks_cnt);
      std::vector<int> snd_cnt(rnks_cnt);
      getRanksSentTo(this,rnk,&snd_to[0]);
      getUnitsSentTo(this,rnk,&snd_cnt[0]);
      std::vector<int> rnks_cnts(sz);
      rnks_cnts[rnk] = rnks_cnt;
      MPI_Allgather(&rnks_cnt,1,mpi_type<int>(),
                    &rnks_cnts[0],1,mpi_type<int>(),
                    cm);
      int ttl_cnt = rnks_cnts[0];
      int offsets[ttl_cnt];
      offsets[0] = 0;
      for(int ii = 1; ii < sz; ii++)
      {
        ttl_cnt += rnks_cnts[ii];
        offsets[ii] = rnks_cnts[ii] + offsets[ii-1];
      }
      int snd_tos[ttl_cnt];
      int snd_cnts[ttl_cnt];
      MPI_Allgatherv(&snd_to[0],rnks_cnt,mpi_type<int>(),
                     &snd_tos[0],&rnks_cnts[0],&offsets[0],mpi_type<int>(),
                     cm);
      MPI_Allgatherv(&snd_cnt[0],rnks_cnt,mpi_type<int>(),
                     &snd_cnts[0],&rnks_cnts[0],&offsets[0],mpi_type<int>(),
                     cm);
      for(int ii = 0; ii < sz; ii++)
      {
        int cnt = rnks_cnts[ii];
        for(int jj = 0; jj < cnt; jj++)
          (*this)(ii,snd_tos[jj+offsets[ii]]) = snd_cnts[jj+offsets[ii]];
      }
      assembled = true;
      result = 0;
    }
    else
      result = 1;
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
  CommPattern * CommPattern_CreateInverted(const CommPattern * pattern)
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
}
