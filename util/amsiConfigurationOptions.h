#ifndef AMSI_AMSICONFIGURATIONOPTIONS_H
#define AMSI_AMSICONFIGURATIONOPTIONS_H
#include <string>
#include <vector>
#include <optional>
namespace mt {
  class CategoryNode;
}
// need functions to parse amsi configuration from file
namespace amsi {
  struct MultiscaleOptions {
    struct Scale {
      std::string name;
      int nprocs;
    };
    struct Relation {
      std::string scale1;
      std::string scale2;
    };
    std::vector<Scale> scales;
    std::vector<Relation> relations;
  };
  struct AnalysisOptions {
    bool use_petsc{false};
    std::string petsc_options_file;
    std::string results_directory;
  };
  struct AmsiOptions {
    std::optional<MultiscaleOptions> multiscale;
    std::optional<AnalysisOptions> analysis;
  };
  /**
   * Reads Amsi options from model-traits input
   */
  AmsiOptions readAmsiOptions(std::string_view filename);
  /**
   * Reads Amsi options from model-traits input
   */
  AmsiOptions readAmsiOptions(std::istream &yaml);
}  // namespace amsi
#endif  // AMSI_AMSICONFIGURATIONOPTIONS_H
