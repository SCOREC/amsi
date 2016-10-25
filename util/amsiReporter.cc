#include "amsiReporter.h"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iomanip>
#include <limits>
#include <map>
#include <sstream>

#include <mpi.h>

namespace amsi {
  class Log {
  protected:
    std::string name;
    std::stringstream stream;
    double creation_time;
    double last_post;
    bool echod;
    std::string tty;
    void init()
    {
      stream << std::setprecision(std::numeric_limits<double>::digits10+2);
      creation_time = MPI_Wtime();
      last_post = creation_time;
    }
  public:
    Log() : name(), stream(), creation_time(), last_post(), echod(false), tty("")
    {
      init();
    }
    Log(const std::string & nm)
      : name(nm)
      , stream()
      , creation_time()
      , last_post()
      , echod(false)
      , tty("")
    {
      init();
    }
    Log(const std::string & nm, const std::string & tt)
      : name(nm)
      , stream()
      , creation_time()
      , last_post()
      , echod(true)
      , tty(tt)
    {
      init();
    }
    std::stringstream & getStream()
    {
      return stream;
    }
    const std::string & getName() const { return name; }
    const std::string & getTty() const { return tty; }
    bool isEchod() const { return echod; }
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
  std::map<std::string,Log*> logs;
  Log * makeLog(const std::string & nm, const std::string & echo)
  {
    Log * result = NULL;
    result = logs[nm];
    if(result == NULL && echo.empty())
      result = logs[nm] = new Log(nm);
    else if(result == NULL && !echo.empty())
      result = logs[nm] = new Log(nm,echo);
    return result;
  }
  int deleteLog(Log *& l)
  {
    assert(l);
    int result = 0;
    result += (logs.erase(l->getName()) == 0);
    l = NULL;
    return result;
  }
  const std::string & getName(Log * l)
  {
    return l->getName();
  }
  std::iostream & log(Log * l)
  {
    assert(l);
    return l->getStream();
  }
  std::iostream & namedLog(Log * l)
  {
    log(l) << "[" << getName(l) << "] ";
    return log(l);
  }
  double post(Log * l)
  {
    assert(l);
    return l->post();
  }
  double getSincePost(Log * l)
  {
    assert(l);
    return l->sincePost();
  }
  double getElapsedTime(Log * l)
  {
    assert(l);
    return l->elapsed();
  }
  double getInitialTime(Log * l)
  {
    assert(l);
    return l->getCreation();
  }
  void writeToStream(Log * l, std::ostream & out)
  {
    out << l->getStream().str();
    if(l->isEchod())
    {
      std::ofstream echo(l->getTty());
      echo << "\n" << l->getStream().str();
    }
    out.flush();
  }
  void flushToStream(Log * l, std::ostream & out)
  {
    writeToStream(l,out);
    l->clear();
  }
}
