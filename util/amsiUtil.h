#ifndef AMSIUTIL_H_
#define AMSIUTIL_H_
#include "amsiFail.h"
#include "amsiMPI.h"
#include "amsiEnumOps.h"
#include "amsiFileSys.h"
#include "amsiReporter.h"
#include "amsiString.h"
#include "amsiOutput.h"
#include "amsiUtilConfig.h"
namespace amsi {
  [[deprecated(
      "use filesystem object in analysis class")]] extern FileSystemInfo *fs;
  [[deprecated("use filesystem object in analysis class")]] void
  SetGlobalFilesystem(FileSystemInfo *fileinfo);
}  // namespace amsi
#endif
