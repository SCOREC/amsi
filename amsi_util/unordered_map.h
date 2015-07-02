#ifndef UNORDERED_MAP_AMSI_H_
#define UNORDERED_MAP_AMSI_H_

#ifdef BGQ
  #include <boost/unordered/unordered_map.hpp>
#else
  #include <unordered_map>
#endif

#include <mpi.h>

namespace amsi {
  
  class CommPattern;
  class Task;
    
#ifdef BGQ
  using boost::hash;
  typedef boost::unordered_map<size_t,CommPattern*> commpatternmap_type;
#else
  using std::hash;
  typedef std::unordered_map<size_t,CommPattern*> commpatternmap_type;
#endif
  
  template <class T>
    inline void hash_combine(std::size_t& seed, const T& v)
  {	
    hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
  }

} // namespace amsi

#endif
