#ifndef AMSIMETA_H_
#define AMSIMETA_H_

#include "TaskManager.h"
#include "CommunicationManager.h"

#include <amsiUtil.h>

namespace amsi {

  extern TaskManager * tm;
  extern CommunicationManager * cm;
  
  void amsiMetaInit(int,char**);
  void amsiMetaFree();

  void configureFromFile(const std::string &);
}


#endif
