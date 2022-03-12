#include "amsi.h"
#include "amsiAnalysis.h"
#include "amsiConfigurationOptions.h"
#include "test.h"
int main(int argc, char* argv[])
{
  amsi::AnalysisOptions analysis_options = {.use_petsc = true};
  amsi::AmsiOptions options{.analysis = analysis_options};
  [[maybe_unused]] amsi::Amsi amsi{options, argc, argv};
  return 0;
}
