#ifndef MPI_T_H_
#define MPI_T_H_

#include <mpi.h>

#include <cstring> //memcpy
#include <iostream>

extern MPI_Comm AMSI_COMM_WORLD;
extern MPI_Comm AMSI_COMM_LOCAL;

double distributedAverage(double partial_sum, int partial_count, MPI_Comm cm);

template <typename T>
void t_ssend(T & msg, MPI_Datatype type, int to)
{
  MPI_Send(&msg,
	   1,
	   type,
	   to,
	   0,
	   MPI_COMM_WORLD);
}

template <typename T>
void t_srecv(T & msg, MPI_Datatype type, int from)
{
  MPI_Status status;
  MPI_Recv(&msg,
	   1,
	   type,
	   from,
	   0,
	   MPI_COMM_WORLD,
           &status);
}

/// @brief A templated wrapper function around MPI_Send to facilitate easier sending
///        using STL and other similar data-storage containers.
/// @tparam T A storage object from which the plain-old-data (POD) datatype will be sent.
///           This type must support the [] bracket operators combined with the reference 
///           operator & to retrieve the adress of the first piece of contigous data to be
///           sent as well as the .size() member function to return the total number to send.
/// @param msg A reference to storage object from which the data will be sent.
/// @param type An MPI_Datatype describing the POD data being sent.
/// @param to The global process rank to which the data is being sent.
template <typename T>
void t_send(T & msg,MPI_Datatype type, int to)
{
  MPI_Send(&msg[0],
	   msg.size(),
           type,
           to,
           0,
           MPI_COMM_WORLD);
}

/// @brief UNTESTED: A templated wrapper function around MPI_Isend to facilitate easier sending
///        using STL and other similar data-storage containers, see documentation of 
///        t_send for additional info.
template <typename T>
void t_isend(T & msg, MPI_Datatype type, int to)
{
  MPI_Request request;
  MPI_Isend(&msg[0],
            msg.size(),
	    type,
	    to,
	    0,
	    MPI_COMM_WORLD,
	    &request);
}

/// @brief A templated wrapper function around MPI_Recv to facilitate easier recving
///        using STL and other similar storage containers.
/// @tparam T A storage object into which the plain-old-data (POD) datatype will be recved.
/// @param msg A reference to a data container object into which the data will be 
///            recved, must support the .resize(int) member function to allocate space into
///            which the data will be recved, and the bracket [] and reference & operators 
///            to retrieve the memory location of the contigous memory into which the 
///            data will be recved.
/// @param type An MPI_Datatype describing the data to be recved into msg
/// @param from The global rank of the process from which to recv the data
/// @return int The number of pieces of data of type described by T recved by this process.
template <typename T>
int t_recv(T & msg,MPI_Datatype type, int from)
{
  MPI_Status status;
  MPI_Probe(from,
	    0,
	    MPI_COMM_WORLD,
	    &status);

  int count = -1;
  MPI_Get_count(&status, type, &count);
  msg.resize(count);

  MPI_Recv(&msg[0],
	   count,
	   type,
	   from,
	   0,
	   MPI_COMM_WORLD,
	   &status);
  return count;
}


/// @brief UNTESTED: A templated wrapper function around MPI_Irecv to facilitate easier recving
///        using STL and other similar storage containers.
/// @tparam T A storage object into which the plain-old-data (POD) datatype will be recved.
/// @param msg A reference to a data container object into which the data will be 
///            recved, must support the .resize(int) member function to allocate space into
///            which the data will be recved, and the bracket [] and reference & operators 
///            to retrieve the memory location of the contigous memory into which the 
///            data will be recved.
/// @param type An MPI_Datatype describing the data to be recved into msg
/// @param from The global rank of the process from which to recv data
/// @param request An MPI_Request object which will be associated with the MPI_Irecv call
/// @return bool If the communication metadata has not been recv'd and the call to MPI_Irecv 
///         could not be made, this is false and the data has not and will not be recv'd. If
///         this is true, then the communication metadata has been recv'd and the call to MPI_Irecv
///         has been made, though all data may not have been recv'd yet.
template <typename T>
bool t_irecv(T & msg, MPI_Datatype type, int from, MPI_Request & request)
{
  bool result = false;

  int flag = 0;
  MPI_Status status;
  MPI_Iprobe(from,
	     MPI_ANY_TAG,
	     MPI_COMM_WORLD,
	     &flag,
	     &status);
  
  if(flag)
  {
    result = true;
    int count = -1;
    MPI_Get_count(&status,type,&count);
    msg.resize(count);

    MPI_Irecv(&msg[0],
	      count,
	      type,
	      from,
	      MPI_ANY_TAG,
	      MPI_COMM_WORLD,
	      &request);
  }

  return result;
}

#endif
