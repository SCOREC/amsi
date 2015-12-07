#include "CommPattern_Create.h"

#include "DataDistribution.h"

#include <iostream>
#include <limits.h>
#include <cstdlib>

namespace amsi
{
  CommPattern * CommPattern_CreateDistro(DataDistribution & dd, int s1, int s2)
  {
    CommPattern * pattern = NULL;
    if(s1 > 0 && s2 > 0)
    {
      //std::cout << __func__ << " : s1 and s2 : " << s1 << "," << s2 << std::endl;
	int total_data = 0;
	//std::cout << "Data distribution : ";
	for(int ii = 0; ii < s1; ii++)
	{
	  //std::cout << dd[ii] << " ";
	  total_data += dd[ii];
	}
	//std::cout << std::endl;
	
	// calculate the comm pattern
	int even_distro = total_data / s2;
	int extra_distro = total_data % s2;
	
	pattern = new CommPattern(s1,s2);
	int dst = 0;
	int dst_rcv = even_distro + (dst < extra_distro);
	for(int src = 0; src < s1; src++)
	{
	  int src_count = dd[src];
	  while(src_count > 0)
	  {
	    // if there are enough to send all needed to dst
	    if(src_count >= dst_rcv)
	    {
	      (*pattern)(src,dst) = dst_rcv;
	      src_count -= dst_rcv;
	      dst++;
	      dst_rcv = even_distro + (dst < extra_distro);
	    }
	    else if(src_count < dst_rcv)
	    {
	      (*pattern)(src,dst) = src_count;
	      dst_rcv -= src_count;
	      src_count = 0;
	    }
	  }
	}
      }
      else
	pattern = new CommPattern(s1 > 0 ? s1 : 0, s2 > 0 ? s2 : 0);
      return pattern;
    }



} // namespace amsi
