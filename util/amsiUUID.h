#ifndef AMSI_UUID_H_
#define AMSI_UUID_H_
#include "amsiHost.h"
#include <cstdlib>
namespace amsi
{
  typedef size_t uuid;
  template <typename T>
    inline uuid generateUUID(const T & src)
  {
    static hash<T> hasher;
    return hasher(src);
  }
  template <class T>
    inline void combineUUID(uuid & a, T & b)
  {
    static hash<T> hasher;
    a ^= hasher(b) + 0x9e3779b9 + (a << 6) + (a >> 2);
  }
  template <class T>
    inline uuid generateRangeUUID(const T & begin,
                                  const T & end)
  {
    uuid result = 0;
    for(T it = begin; it != end; it++)
      combineUUID(result,*it);
    return result;
  }
}
#endif
