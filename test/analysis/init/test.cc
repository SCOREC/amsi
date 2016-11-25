#include "test.h"
#include "amsiAnalysis.h"
int main(int argc, char ** argv)
{
  amsi::initAnalysis(argc,argv);
  amsi::freeAnalysis();
  return 0;
}
