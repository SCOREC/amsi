#ifndef AMSIUTIL_H_
#define AMSIUTIL_H_
#include "amsiUtilConfig.h"
namespace amsi
{
  void utilInit(int argc, char ** argv);
  void utilFree();
  
  void amsiInit(int argc, char ** argv);
  void amsiFree();
  class amsiInitializer
  {
  public:
    virtual void amsiInit(int,char**) = 0;
    virtual void amsiFree() = 0;
  };
  class amsiUtilInit : public amsiInitializer
  {
  private:
    long util_init_call_count;
  public:
    amsiUtilInit() : util_init_call_count(0) {}
    virtual void amsiInit(int, char**);
    virtual void amsiFree();
  };
  extern amsiInitializer * initializer;
}

#endif
