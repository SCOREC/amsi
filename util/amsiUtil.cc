#include "amsiUtil.h"
namespace amsi {
  //  // global variables YIKES!
  //  std::string options_filename("");
  [[deprecated("use filesystem object in analysis class")]] FileSystemInfo* fs =
      nullptr;
  [[deprecated("use filesystem object in analysis class")]] void
  SetGlobalFilesystem(FileSystemInfo* fileinfo)
  {
    if (fs != nullptr) {
      delete fs;
    }
    // this leaks...but will eventually be gone...
    fs = new FileSystemInfo{*fileinfo};
  }
}  // namespace amsi