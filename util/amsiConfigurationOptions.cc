#include "amsiConfigurationOptions.h"
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include "amsiExceptions.h"
namespace amsi {
  static std::optional<MultiscaleOptions> readMultiscaleOptions(
      const YAML::Node& amsi)
  {
    if (!amsi["multiscale"]) {
      return {};
    }
    auto multiscale_node = amsi["multiscale"];
    MultiscaleOptions multiscale_options;
    if (multiscale_node["scales"]) {
      for (const auto& scale : multiscale_node["scales"]) {
        auto scale_name = scale.first.as<std::string>();
        auto nprocs = scale.second.as<int>();
        multiscale_options.scales.push_back(
            {.name = scale_name, .nprocs = nprocs});
      }
    }
    if (multiscale_node["relations"]) {
      for (const auto& relation : multiscale_node["relations"]) {
        auto first_scale = relation.first.as<std::string>();
        for (const auto& second_scale_node : relation.first) {
          auto second_scale = second_scale_node.as<std::string>();
          multiscale_options.relations.push_back(
              {.scale1 = first_scale, .scale2 = second_scale});
        }
      }
    }
    return multiscale_options;
  }
  static std::optional<AnalysisOptions> readAnalysisOptions(
      const YAML::Node& amsi)
  {
    if (!amsi["analysis"]) {
      return {};
    }
    auto analysis_node = amsi["analysis"];
    AnalysisOptions analysis_options;
    if (analysis_node["results directory"]) {
      analysis_options.results_directory =
          analysis_node["results directory"].as<std::string>();
    }
    if (analysis_node["petsc"]) {
      auto petsc_node = analysis_node["petsc"];
      if (petsc_node["enabled"]) {
        analysis_options.use_petsc = petsc_node["enabled"].as<bool>();
      }
      else {
        analysis_options.use_petsc = false;
      }
      if (petsc_node["options file"]) {
        analysis_options.petsc_options_file =
            petsc_node["options file"].as<std::string>();
      }
    }
    return analysis_options;
  }
  AmsiOptions readAmsiOptions(std::istream& yaml)
  {
    auto yaml_file = YAML::Load(yaml);
    if (!yaml_file["amsi"]) {
      throw amsi_error{"amsi options not found in yaml file"};
    }
    auto amsi = yaml_file["amsi"];
    AmsiOptions options;
    auto multiscale_options = readMultiscaleOptions(amsi);
    auto analysis_options = readAnalysisOptions(amsi);
    options.multiscale = multiscale_options;
    options.analysis = analysis_options;
    return options;
  }
  AmsiOptions readAmsiOptions(std::string_view filename)
  {
    std::ifstream yaml{filename};
    if (!yaml.is_open()) {
      std::stringstream ss;
      ss << "cannot open amsi options file " << filename;
      throw amsi_error{ss.str()};
    }
    return readAmsiOptions(yaml);
  }
}  // namespace amsi