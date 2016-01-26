#include "amsiFileSys.h"
#include <iostream> // temp
namespace amsi
{
  const std::string getpwd()
  {
    char cwd[FILENAME_MAX];
    getcwd(&cwd[0],sizeof(cwd));
    return std::string(cwd);
  }
  void printInfo(FileSystemInfo * fsi, std::ostream & out)
  {
    out << "Executing in " << fsi->getcwd() << std::endl;
    out << "Results written to " << fsi->getResultsDir() << std::endl;
  }
  bool isRelativePath(const std::string & pth)
  {
    bool result = false;
    if(pth.length() > 0)
      result = pth[0] != '/';
    return result;
  }
  bool isAbsolutePath(const std::string & pth)
  {
    return !isRelativePath(pth);
  }
  FileSystemInfo::FileSystemInfo(const std::string & rs)
    : cwd(getpwd())
    , rslts()
    , rslts_rel(false)
  {
    rslts_rel = isRelativePath(rs);
    if(rslts_rel)
      rslts = cwd + "/" + rs;
    else
      rslts = rs;
  }
}
