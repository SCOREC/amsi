#ifndef AMSI_FILE_SYS_H_
#define AMSI_FILE_SYS_H_
#include <fstream>
#include <unistd.h>
namespace amsi
{
  class FileSystemInfo
  {
  private:
    std::string cwd;
    std::string rslts;
    bool rslts_rel;
  public:
    FileSystemInfo(const std::string & rs);
    const std::string & getcwd() {return cwd;}
    const std::string & getResultsDir() {return rslts;}
  };
  void describeFSInfo(FileSystemInfo * fsi,
                      std::ostream & out);
  bool isRelativePath(const std::string & pth);
  bool isAbsolutePath(const std::string & pth);
};
#endif
