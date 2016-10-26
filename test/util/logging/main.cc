#include "amsiReporter.h"
#include "amsiTee.h"
#include <fstream>
int main(int argc, char * argv[])
{
  std::string arg1(argv[1]);
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
  amsi::Log * echo = amsi::makeLog("echo_log", arg1);
  amsi::namedLog(echo) << " log created: " << amsi::getInitialTime(echo) << std::endl;
  amsi::namedLog(echo) << " this should be written to a tty as specified by the first input argument: " << arg1 <<  std::endl;
  {
    std::ofstream e("echo.log");
    amsi::flushToStream(echo,e);
  }
  amsi::deleteLog(echo);
  result += (echo != NULL);
  amsi::Log * echo2 = amsi::makeLog("tee_echo_log");
  amsi::namedLog(echo2) << " log created: " << amsi::getInitialTime(echo2) << std::endl;
  amsi::namedLog(echo2) << " this should be written to a tty as specified by the first input argument: " << arg1 <<  std::endl;
  {
    std::ofstream e1("tee_echo.log");
    std::ofstream e2(arg1);
    amsi::teestream t(e1,e2);
    amsi::flushToStream(echo2,t);
  }
  amsi::deleteLog(echo2);
  result += (echo2 != NULL);
  return result;
}
