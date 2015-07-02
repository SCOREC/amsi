#ifndef AMSIREPORTER_H_
#define AMSIREPORTER_H_

#include <string>
#include <iostream>

namespace amsi {
  int log_new(const std::string &);
  std::iostream & log(const std::string &);
  int log_print(const std::string &, std::ostream &);
  int log_delete(const std::string &);
  double log_start_time(const std::string &);
}

#endif
