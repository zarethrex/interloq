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
    os << "name: " << inter.name << "\n";
    os << "interlocking: \n";
    os << "\n";
    for (const auto &[id, lever] : inter.leverframe) {
      os << id << ":\n";
      os << "  "
         << "name: " << lever.name << "\n";
      os << "  "
         << "dependencies: \n";
      for (const auto &[id, dependency] : lever.dependencies) {
        os << "    " << id << ":\n";
        os << "      "
           << "required_state: "
           << (dependency.required_state ? "true" : "false") << "\n";
        os << "      "
           << "target: " << dependency.target << "\n";
        os << "      "
           << "alt_choices:\n";
        for (const auto &alt : dependency.alt_choices) {
          os << "        -" << alt << "\n";
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
