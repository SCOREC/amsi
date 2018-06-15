#include "amsiReporter.h"
#include "amsiObserver.h"
#include "pystring.h"
#include "amsiTee.h"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <limits>
#include <map>
#include <sstream>
#include <mpi.h>
namespace amsi
{
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
    std::stringstream & getBuffer() { return stream; }
    const std::string & getName() const { return name; }
    double getCreation() const { return creation_time; }
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
  const std::string & getName(Log l)
  {
    return l->getName();
  }
  std::iostream & log(Log l)
  {
    return l->getBuffer();
  }
  std::iostream & namedLog(Log l)
  {
    log(l) << "[" << getName(l) << "] ";
    return log(l);
  }
  double post(Log l)
  {
    return l->post();
  }
  double getSincePost(Log l)
  {
    return l->sincePost();
  }
  double getElapsedTime(Log l)
  {
    return l->elapsed();
  }
  double getInitialTime(Log l)
  {
    return l->getCreation();
  }
  void writeToStream(Log l, std::ostream & out)
  {
    out << l->getBuffer().str();
    out.flush();
  }
  void flushToStream(Log l, std::ostream & out)
  {
    writeToStream(l,out);
    l->clear();
  }
  static std::map<std::string,std::ostream*> log_streams;
  std::ostream & getLogStream(Log l)
  {
    const std::string & nm = getName(l);
    if(log_streams.find(nm) != log_streams.end())
      std::cerr << "WARNING: Log " << nm << " has no registered output streams, defaulting to std::cout!" << std::endl;
    else
      return *(log_streams[nm]);
    return std::cout;
  }
  void addLogStream(Log l, std::ostream & strm)
  {
    const std::string & nm = getName(l);
    if(log_streams.find(nm) == log_streams.end())
      log_streams[nm] = &strm;
  }
  void write(Log l)
  {
    writeToStream(l,getLogStream(l));
  }
  void flush(Log l)
  {
    flushToStream(l,getLogStream(l));
  }
  Log activateLog(const std::string & nm)
  {
    Log result;
    result = logs[nm];
    if(result == NULL)
    {
      //std::cout << "Creating log: " << nm << std::endl;
      result = logs[nm] = Log(new log_class(nm));
    }
    return result;
  }
  int deleteLog(Log & l)
  {
    int dltd = 0;
    const std::string nm(l->getName());
    dltd += (logs.erase(nm) == 0); // remove the pointer from the map
    l.reset(); // either delete the log or decrement the shared_ptr counter
    if(l.get() == NULL)
      log_streams.erase(nm);
    return dltd;
  }
  /*
  void configureLogStreams(std::istream & cnfg)
  {
    std::string ln;
    std::vector<std::string> tkns;
    while(std::getline(cnfg,ln))
    {
      ln = pystring::strip(ln);
      if(pystring::startswith(ln,std::string("#")))
        return;
      pystring::split(ln,tkns," ");
      activateLog(tkns[0]);
      std::ostream * strm = NULL;
      for(size_t ii = 1; ii < tkns.size(); ++ii)
      {
        if(tkns[ii] == "file")
          std::fstream strm(tkns[++ii]);
        else if(tkns[ii] == "stdout")
          std::cout;
        else if(tkns[ii] == "stderr");
          std::cerr;
      }
    }
  }
  */
}
