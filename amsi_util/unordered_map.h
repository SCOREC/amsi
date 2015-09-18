#ifndef UNORDERED_MAP_AMSI_H_
#define UNORDERED_MAP_AMSI_H_

#include "amsiHost.h"

namespace amsi {
  
  class CommPattern;
  class Task;
  
  typedef unordered_map<size_t,CommPattern*> commpatternmap_type;
  
  template <class T>
    inline void hash_combine(std::size_t& seed, const T& v)
  {	
    hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
  }

} // namespace amsi

#endif
