#include "CommPattern.h"

#include <cstring> // for memset
#include <iomanip>
#include <iostream>

namespace amsi {


  std::pair<int,int> coupledInfoByIndex(CommPattern * cp,
					 CommPattern::SendRecv send_recv,
					 int rank,
					 int index)
  {
    std::pair<int,int> result;
    int remainder = 0;
    int coupled_rank = -1;
    int num_recvers = cp->getRecvTaskSize();
    int num_senders = cp->getSendTaskSize();
    if(send_recv == CommPattern::SENDER)
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
    else if(send_recv == CommPattern::RECVER)
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

    /// @brief Default constructor.
    /// @param s1_ The number of processes in the ProcessSet related to the sending task
    /// @param s2_ The number of processes in the ProcessSet related to the recving task
    CommPattern::CommPattern(int s1_, int s2_) :
      s1(s1_),
      s2(s2_),
      pattern(),
      valid(true),
      assembled(false),
      reconciled(false)
    { 
      pattern = new int[s1 * s2];
      memset(&pattern[0],0, sizeof(int) * s1 * s2);
    }

    /// @brief Retrieve a count for a specific send/recv pair
    /// @param rank1 The sending task-rank
    /// @param rank2 The recving task-rank
    /// @return int The number of pieces of data to be communicated
    int CommPattern::GetDataCount(int rank1, int rank2) const
    {
      return pattern[rank1 * s2 + rank2];
    }

    /// @brief Retrieve a vector representing the number of pieces of data to be 
    ///        sent to each rank in the recving task from a rank in the sending task
    /// @param rank The task-rank of the sending process
    /// @param send_to A vector containing, for each process in the recving task, 
    ///                the number of pieces of data to be sent to that process
    void CommPattern::GetSendTo(int rank, std::vector<int>& send_to) const
    {
      int offset = rank * s2;
      for(int ii = 0; ii < s2; ii++)
	send_to.push_back(pattern[offset + ii]);
    }
    
    /// @brief Retrieve a vector representing the number of pieces of data to be
    ///        recved from each rank in the sending task to a rank in the recving task
    /// @param rank The task-rank of the recving process
    /// @param recv_from A vector containing, for each process in the sending task,
    ///                  the number of pieces of data to be recved from that process
    void CommPattern::GetRecvFrom(int rank, std::vector<int> & recv_from) const
    {
      for(int ii = 0; ii < s1; ii++)
      {
	int offset = ii * s2;
	recv_from.push_back(pattern[offset + rank]);
      }
    }
    
    /// @brief Access/Modification operator, analogous to the .GetDataCount(int,int) member function
    int& CommPattern::operator()(int r1, int r2)
    {
      assembled = false;
      return pattern[r1 * s2 + r2];
    }
    
    /// @brief Access operator, analogous to the .GetDataCount(int,int) member function
    int CommPattern::operator()(int r1, int r2) const
    {
      return pattern[r1 * s2 + r2];
    }

    /// @brief Assemble a CommPattern across a given MPI_Comm (typically only called by another AMSI object.
    /// @param MPI_Comm The comm across which to assemble the CommPattern (should be the MPI_Comm containing the 
    ///                 ranks associated with the sending task.
    /// @param rank The task-rank of the local process.
    int CommPattern::Assemble(MPI_Comm comm, int rank)
    {
      int result = 0;
      if(!assembled)
      {
	std::vector<int> send_to;
	GetSendTo(rank,send_to); // get everything locally sent

      //std::cout << "Assembling!" << send_to.size() << std::endl;
      /* 
	 std::cout << "MPI_Allgather(" 
	 << &send_to[0] << ", "
	 << send_to.size() << ", MPI_INTEGER, " 
	 << &pattern[0] << " ,"
	 << send_to.size() << ", MPI_INTEGER, comm" << std::endl;
      */

#       ifdef CORE

        int size, recv_from;
        void* recv;
        size_t recv_size;
        PCU_Switch_Comm(comm);
        PCU_Comm_Begin();
        MPI_Comm_size(comm,&size);
        for(int i=0;i<size;i++)
          if(i!=rank)
            PCU_Comm_Write(i, &send_to[0], send_to.size()*sizeof(int));
        PCU_Comm_Send();
        while(PCU_Comm_Read(&recv_from,&recv,&recv_size))
          memcpy(&pattern[recv_from*send_to.size()],recv,recv_size);

#       else

	MPI_Allgather(&send_to[0],send_to.size(),MPI_INTEGER,
		      &pattern[0],send_to.size(),MPI_INTEGER,
		      comm);

#       endif

	assembled = true;
	result = 0;
      }
      else
	result = 1;

      return result;
    }

    void CommPattern::Reconcile()
    {
/*
      if(!reconciled)
      {
	if(sender)
	{
	  //int to_send = (t2_size / t1_size) + (task_rank < (t2_size % t1_size));
	  
	  for(int ii = 0; ii < ranks

	  for(int ii = 0; ii < to_send; ii++)
	  {
	    int send_to = task_rank + (ii * t1_size);
	    std::vector<int> recv_from;
	    GetRecvFrom(send_to,recv_from);
	    t_send(recv_from,MPI_INTEGER,t2->LocalToGlobalRank(send_to));
	  }
	}
	else
	{
	  int recv_from = task_rank % t1_size;
	  std::vector<int> recv_count;
	  t_recv(recv_count,MPI_INTEGER,t1->LocalToGlobalRank(recv_from));
	  for(int ii = 0; ii < recv_count.size(); ii++)
	    (*recv_pattern)(ii,task_rank) = recv_count[ii];
	}
	reconciled = true;
      }
*/
    }

    std::ostream& operator<<(std::ostream& os, const CommPattern& obj)
    {
      os << "CommPattern: " << std::endl;
      for(int ii = 0; ii < obj.s1; ii++)
      {
	for(int jj = 0; jj < obj.s2; jj++)
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
      result = new CommPattern(pattern->s2,pattern->s1);

      for(int ii = 0; ii < pattern->s1; ii++)
	for(int jj = 0; jj < pattern->s2; jj++)
	  (*result)(jj,ii) = (*pattern)(ii,jj);


      //std::cout << (*result);
      
      return result;
    }

} // namespace amsi
