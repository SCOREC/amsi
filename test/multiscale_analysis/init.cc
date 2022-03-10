#include "amsi.h"
#include "amsiAnalysis.h"
#include "amsiConfigurationOptions.h"
#include "test.h"
int main(int argc, char* argv[])
{
  amsi::AmsiOptions options{.multiscale_analysis = true, .use_petsc = true};
  [[maybe_unused]] amsi::Amsi amsi{options, argc, argv};
  return 0;
}
