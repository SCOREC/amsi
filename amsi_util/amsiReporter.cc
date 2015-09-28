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

  std::string results_dir("results/");

  class Log {
  protected:
    std::string name;
    std::stringstream stream;
    double creation_time;
    double last_post;
  public:
    Log() : name(), stream()
    {
      stream << std::setprecision(std::numeric_limits<double>::digits10+2);
      creation_time = MPI_Wtime();
      last_post = creation_time;
    }

    Log(const std::string & nm) : name(nm), stream()
    {
      stream << std::setprecision(std::numeric_limits<double>::digits10+2);
      creation_time = MPI_Wtime();
      last_post = creation_time;
    }
    
    std::iostream & getStream()
    {
      return stream;
    }

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
    
    void writeFStream(const std::string & filename) const
    {
      std::fstream out;
      out.open(filename.c_str(), std::fstream::out | std::fstream::app);
      assert(out.is_open());
      out << stream.str();
      out.flush();
      out.close();
    }

    void writeMPI(const std::string & filename) const
    {
	
    }
  };

  std::map<std::string,Log*> logs;

  Log * makeLog(const std::string & nm)
  {
    Log * result = NULL;
    result = logs[nm];
    if(result == NULL)
      result = logs[nm] = new Log();
    return result;
  }

  int deleteLog(Log * l)
  {
    assert(l);
    int result = 0;
    result += (logs.erase(l->getName()) == 0);
    return result;
  }

  std::iostream & log(Log * l)
  {
    assert(l);
    return l->getStream();
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

  void flush2FStream(Log * l, const std::string & out)
  {
    std::string fnm(results_dir + out);
    l->writeFStream(fnm);
    l->clear();
  }

  void flush2MPI(Log * l, const std::string & out)
  {
    l->writeMPI(out);
    l->clear();
  }


  
}
