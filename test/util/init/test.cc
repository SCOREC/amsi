#include "test.h"
#include "amsiUtil.h"
int main(int argc, char ** argv)
{
  amsi::initUtil(argc,argv);
  amsi::freeUtil();
  return 0;
}
