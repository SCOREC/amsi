#include "DataDistribution.h"

#ifdef ZOLTAN
#include <zoltan.h>
#endif

namespace amsi {
  
  DataDistribution::DataDistribution(int size) :
    assembled(false),
    valid(false),
    wgts(size),
    dd(size)
  {  }
  
# ifdef ZOLTAN
  DataDistribution::DataDistribution(int size, Zoltan_Struct * z) :
    assembled(false),
    valid(false),
    wgts(size),
    dd(size),
    zs(z)
  { }
#endif

  int DataDistribution::Assemble(MPI_Comm comm)
  {
    int comm_rank = 0;
    MPI_Comm_rank(comm,&comm_rank);
    if(!assembled)
    {
#     ifdef CORE
      int size;
      int recv_from;
      void* recv;
      size_t recv_size;
      
      PCU_Switch_Comm(comm);
      PCU_Comm_Begin();
      
      MPI_Comm_size(comm,&size);
      
      for(int i=0;i<size;i++)
      {
	if(i!=comm_rank){
	  PCU_Comm_Write(i, &dd[comm_rank], sizeof(int));
	}
      }
      
      PCU_Comm_Send();
      
      while(PCU_Comm_Read(&recv_from,&recv,&recv_size)){
	dd[recv_from] = *(int*)recv;
      }
#endif

      for(unsigned ii = 0; ii < dd.size(); ii++)
	wgts[ii].resize(dd[ii]);
      
      assembled = true;
    }
    
    return assembled;
  }

  int DataDistribution::operator [](unsigned index) const
  {
    return dd[index];
  }
  
  int& DataDistribution::operator [](unsigned index)
  {
    assembled = false;
    return dd[index];
  }

  void DataDistribution::setWeight(unsigned index, unsigned sub_index, double value)
  {
    if(!assembled)
    {
      // HACKY really should assemble here... but we're unlikely to hit this case
      wgts[index].resize(dd[index]);
      valid = true;
    }
    wgts[index][sub_index] = value;
  }

  double DataDistribution::getWeight(unsigned index, unsigned sub_index)
  {
    if(sub_index+1 > wgts[index].size())
      wgts[index].resize(sub_index+1,0.0);
    return wgts[index][sub_index];
  }

} // namespace amsi
