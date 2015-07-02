#include "amsiReporter.h"

#include <iomanip>
#include <limits>
#include <map>
#include <sstream>

#include <mpi.h>

namespace amsi {

  class Log {
  private:
    std::stringstream stream;
  public:
    Log() : stream()
      {
	stream << std::setprecision(std::numeric_limits<double>::digits10+2);
      }
    
    std::iostream & getStream()
      {
	return stream;
      }
    int print(std::ostream & out)
      {
	out << stream.str();
	out.flush();
	stream.str("");
      }
  };

  std::map<std::string,Log*> logs;
  std::map<std::string,double> start_times;

  // todo: add existence checking
  int log_new(const std::string & nm)
  {
    int result = 0;
    logs[nm] = new Log();
    start_times[nm] = MPI_Wtime();
    result += logs[nm] != NULL;
    return result;
  }

  std::iostream & log(const std::string & nm)
  {
    return logs[nm]->getStream();
  }

  int log_print(const std::string & nm, std::ostream & out)
  {
    int result = 0;
    logs[nm]->print(out);
    return result;
  }

  int log_delete(const std::string & nm)
  {
    int result = 0;
    result += (logs.erase(nm) == 0);
    return result;
  }

  double log_start_time(const std::string & nm)
  {
    double result = 0.0;
    result = start_times[nm];
    return result;
  }
  
  
}
