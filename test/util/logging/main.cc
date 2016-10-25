#include "amsiReporter.h"
#include <fstream>
int main(int argc, char * argv[])
{
  int result = 0;
  amsi::Log * unamed = amsi::makeLog("");
  amsi::log(unamed) << amsi::getName(unamed) << " log created: " << amsi::getInitialTime(unamed) << std::endl;
  {
    std::ofstream un("unamed.log");
    amsi::flushToStream(unamed,un);
  }
  amsi::deleteLog(unamed);
  result += (unamed != NULL);
  amsi::Log * named = amsi::makeLog("named_log");
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
  amsi::Log * echo = amsi::makeLog("echo_log", std::string(argv[1]));
  amsi::namedLog(echo) << " log created: " << amsi::getInitialTime(echo) << std::endl;
  amsi::namedLog(echo) << " this should be written to a tty as specified by the first input argument: " << argv[1] << std::endl;
  {
    std::ofstream e("echo.log");
    amsi::flushToStream(echo,e);
  }
  amsi::deleteLog(echo);
  result += (echo != NULL);
  return result;
}
