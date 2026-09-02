#pragma once

#include "fkYAML/node.hpp"
#include <filesystem>
#include <fstream>
#include <magic_enum/magic_enum.hpp>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

namespace interloq {
enum class LeverType {
  StopSignal,
  DistantSignal,
  Points,
  PointLock,
  Crossing,
  Detonator,
  Spare,
};
struct LeverDependency {
  unsigned int target;
  bool required_state;
  std::vector<unsigned int> alt_choices;
};
struct LeverDefinition {
  unsigned int id;
  LeverType type;
  std::string name;
  std::map<unsigned int, LeverDependency> dependencies;
};
struct InterlockDefinition {
  std::string name;
  std::map<unsigned int, LeverDefinition> leverframe;
  static InterlockDefinition yaml_load(const std::filesystem::path &input_file);
  friend std::ostream &operator<<(std::ostream &os,
                                  const InterlockDefinition &inter) {
    os << "name: " << inter.name << "\n"
       << "interlocking: \n\n";
    for (const auto &[id, lever] : inter.leverframe) {
      os << id << ":\n"
         << "  name: " << lever.name << "\n"
         << "  dependencies: \n";
      for (const auto &[dep_id, dependency] : lever.dependencies) {
        os << "    " << dep_id << ":\n"
           << "      required_state: "
           << (dependency.required_state ? "true" : "false") << "\n"
           << "      target: " << dependency.target << "\n"
           << "      alt_choices:\n";
        for (const auto &alt : dependency.alt_choices) {
          os << "        - " << alt << "\n";
        }
      }
    }
    return os;
  }
};

void to_node(fkyaml::node &n, const LeverDefinition &lever);
void from_node(const fkyaml::node &n, LeverDefinition &lever);
void to_node(fkyaml::node &n, const LeverDependency &dependency);
void from_node(const fkyaml::node &n, LeverDependency &lever);
void to_node(fkyaml::node &n, const InterlockDefinition &dependency);
void from_node(const fkyaml::node &n, InterlockDefinition &lever);
}; // namespace interloq
