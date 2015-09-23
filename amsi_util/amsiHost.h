#ifndef AMSIHOST_H_
#define AMSIHOST_H_

#include "amsiUtilConfig.h"

#ifdef BGQ
  #include <boost/unordered/unordered_map.hpp>
  #include <boost/functional/hash.hpp>
  using boost::hash;
  using boost::unordered_map;
#elif defined SCOREC
  #include <unordered_map>
  #include <functional>
  using std::hash;
  using std::unordered_map;
#endif

#endif
