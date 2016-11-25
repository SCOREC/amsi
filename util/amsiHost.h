#ifndef AMSIHOST_H_
#define AMSIHOST_H_
#include "amsiUtilConfig.h"
#ifdef BGQ
#include <boost/unordered/unordered_map.hpp>
#include <boost/functional/hash.hpp>
using boost::hash;
using boost::unordered_map;
#include <tr1/memory>
using std::tr1::shared_ptr;
#else
#include <unordered_map>
#include <functional>
using std::hash;
using std::unordered_map;
#include <memory>
using std::shared_ptr;
#endif
#include <climits>
int is_big_endian(void);
template <typename T>
T swap_endian(T u);
#include "amsiHost_impl.h"
#endif
