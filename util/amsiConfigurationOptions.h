#ifndef AMSI_AMSICONFIGURATIONOPTIONS_H
#define AMSI_AMSICONFIGURATIONOPTIONS_H
#include <string>
#include <vector>
// need functions to parse amsi configuration from file
namespace amsi {
  class AmsiOptions {
    public:
    struct Scale {
      std::string name;
      int nprocs;
    };
    struct Relation {
      std::string scale1;
      std::string scale2;
    };
    bool multiscale_analysis{false};
    std::vector<Scale> scales;
    std::vector<Relation> relations;
    // Filepath?
    std::string results_directory;
    bool use_petsc{true};
    std::string petsc_options_file;
  };
}  // namespace amsi
#endif  // AMSI_AMSICONFIGURATIONOPTIONS_H
