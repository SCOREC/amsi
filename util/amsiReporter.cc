#include "amsiReporter.h"
#include "amsiObserver.h"
#include <algorithm>
#include <cassert>
#include <fstream>
#include <iomanip>
#include <limits>
#include <map>
#include <sstream>
#include <mpi.h>
namespace amsi {
  class log_class
  {
  protected:
    std::string name;
    std::stringstream stream;
    double creation_time;
    double last_post;
    void init()
    {
      stream << std::setprecision(std::numeric_limits<double>::digits10+2);
      creation_time = MPI_Wtime();
      last_post = creation_time;
    }
  public:
    log_class() : name(), stream(), creation_time(), last_post()
    {
      init();
    }
    log_class(const std::string & nm)
      : name(nm)
      , stream()
      , creation_time()
      , last_post()
    {
      init();
    }
    std::stringstream & getStream() { return stream; }
    const std::string & getName() const { return name; }
    const double getCreation() const { return creation_time; }
    double post() { return last_post = MPI_Wtime(); }
    double sincePost()
    {
      double result = MPI_Wtime();
      result -= last_post;
      return result;
    }
    double elapsed() const
    {
      double result = MPI_Wtime();
      result -= creation_time;
      return result;
    }
    void clear()
    {
      stream.str("");
    }
  };
  static std::map<std::string,Log> logs;
  Log activateLog(const std::string & nm)
  {
    Log result = NULL;
    result = logs[nm];
    if(result == NULL)
      result = logs[nm] = std::make_shared<log_class>(nm);
    return result;
  }
  int deleteLog(Log & l)
  {
    assert(l);
    int result = 0;
    result += (logs.erase(l->getName()) == 0);
    l.reset(); // either delete the log or decrement the shared_ptr counter
    return result;
  }
  const std::string & getName(Log l)
  {
    return l->getName();
  }
  std::iostream & log(Log l)
  {
    assert(l);
    return l->getStream();
  }
  std::iostream & namedLog(Log l)
  {
    log(l) << "[" << getName(l) << "] ";
    return log(l);
  }
  double post(Log l)
  {
    assert(l);
    return l->post();
  }
  double getSincePost(Log l)
  {
    assert(l);
    return l->sincePost();
  }
  double getElapsedTime(Log l)
  {
    assert(l);
    return l->elapsed();
  }
  double getInitialTime(Log l)
  {
    assert(l);
    return l->getCreation();
  }
  void writeToStream(Log l, std::ostream & out)
  {
    out << l->getStream().str();
    out.flush();
  }
  void flushToStream(Log l, std::ostream & out)
  {
    writeToStream(l,out);
    l->clear();
  }
}
