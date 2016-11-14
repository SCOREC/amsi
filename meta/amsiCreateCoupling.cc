#include "amsiCreateCoupling.h"
#include "amsiCoupling.h"
#include "amsiDataDistribution.h"
#include <cassert>
#include <iostream>
#include <limits.h>
#include <cstdlib>
namespace amsi
{
  CommPattern * CommPattern_CreateDistro(DataDistribution & dd, int s1, int s2)
  {
    assert(s1 > 0);
    assert(s2 > 0);
    assert(dd.isAssembled());
    CommPattern * pattern = new FullCommPattern(s1,s2);
    int ttl_cnt = 0;
    for(int ii = 0; ii < s1; ii++)
      ttl_cnt += dd[ii];
    // calculate the comm pattern
    int even_distro = ttl_cnt / s2;
    int extra_distro = ttl_cnt % s2;
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
    return pattern;
  }
}
