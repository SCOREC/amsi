#ifndef AMSI_REPORTER_H_
#define AMSI_REPORTER_H_
#include <memory>
#include <string>
#include <iostream>
namespace amsi
{
  extern std::string results_dir;
  class log_class;
  typedef std::shared_ptr<log_class> Log;
  Log activateLog(const std::string & nm);
  int deleteLog(Log &);
  const std::string & getName(Log);
  std::iostream & log(Log);
  std::iostream & namedLog(Log);
  double post(Log);
  double getSincePost(Log);
  double getElapsedTime(Log);
  double getInitialTime(Log);
  void writeToStream(Log, std::ostream &);
  void flushToStream(Log, std::ostream &);
}
#endif
