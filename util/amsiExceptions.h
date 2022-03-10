#ifndef AMSI_AMSIEXCEPTIONS_H
#define AMSI_AMSIEXCEPTIONS_H
#include <stdexcept>
class mpi_error : public std::logic_error {
  public:
  mpi_error(const std::string& s) : std::logic_error(s) {}
  mpi_error(const char* s) : std::logic_error(s) {}
};
class amsi_error : public std::logic_error {
  public:
  explicit amsi_error(const std::string& s) : std::logic_error(s) {}
  explicit amsi_error(const char* s) : std::logic_error(s) {}
};
#endif  // AMSI_AMSIEXCEPTIONS_H
