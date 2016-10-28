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
  const std::string & getName(Log);
  std::iostream & log(Log);
  std::iostream & namedLog(Log);
  double post(Log);
  double getSincePost(Log);
  double getElapsedTime(Log);
  double getInitialTime(Log);
  void writeToStream(Log, std::ostream &);
  void flushToStream(Log, std::ostream &);
  std::ostream & getLogStream(Log);
  void addLogStream(Log,std::ostream &);
  void write(Log);
  void flush(Log);
  Log activateLog(const std::string & nm);
  int deleteLog(Log &);
}
#endif
