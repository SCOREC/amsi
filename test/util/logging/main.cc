#include "amsiMPI.h"
#include "amsiReporter.h"
#include "amsiTee.h"
#include <fstream>
int main(int argc, char * argv[])
{
  MPI_Init(&argc,&argv);
  std::string arg1(argv[1]);
  int result = 0;
  amsi::Log unamed = amsi::activateLog("");
  amsi::log(unamed) << amsi::getName(unamed) << " log created: " << amsi::getInitialTime(unamed) << std::endl;
  {
    std::ofstream un("unamed.log");
    amsi::flushToStream(unamed,un);
  }
  amsi::deleteLog(unamed);
  result += (unamed != NULL);
  amsi::Log named = amsi::activateLog("named_log");
  amsi::namedLog(named) << " log created: " << amsi::getInitialTime(named) << std::endl;
  amsi::namedLog(named) << " elapsed time: " << amsi::getElapsedTime(named) << std::endl;
  amsi::namedLog(named) << " post: " << amsi::post(named) << std::endl;
  amsi::namedLog(named) << " since post: " << amsi::getSincePost(named) << std::endl;
  {
    std::ofstream n("named.log");
    amsi::flushToStream(named,n);
  }
  amsi::deleteLog(named);
  result += (unamed != NULL);
  amsi::Log echo = amsi::activateLog("tee_echo_log");
  amsi::namedLog(echo) << " log created: " << amsi::getInitialTime(echo) << std::endl;
  amsi::namedLog(echo) << " this should be written to a tty as specified by the first input argument: " << arg1 <<  std::endl;
  {
    std::ofstream e1("tee_echo.log");
    std::ofstream e2(arg1);
    amsi::teestream t(e1,e2);
    amsi::flushToStream(echo,t);
  }
  amsi::deleteLog(echo);
  result += (echo != NULL);
  MPI_Finalize();
  return result;
}
