#ifndef AMSIREPORTER_H_
#define AMSIREPORTER_H_
#include <string>
#include <iostream>
namespace amsi
{
  extern std::string results_dir;
  class Log;
  Log * makeLog(const std::string &);
  int deleteLog(Log *);
  std::iostream & log(Log *);
  double post(Log *);
  double getSincePost(Log *);
  double getElapsedTime(Log *);
  double getInitialTime(Log *);
  void flush2FStream(Log *, const std::string &);
  void flush2MPI(Log *, const std::string &);
}
#endif
