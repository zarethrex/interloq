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
};
struct LeverDefinition {
  unsigned int id;
  LeverType type;
  std::string name;
  std::optional<std::vector<LeverDependency>> dependencies{std::nullopt};
};
struct InterlockDefinition {
  std::string name;
  std::map<unsigned int, LeverDefinition> leverframe;
  static InterlockDefinition yaml_load(const std::filesystem::path &input_file);
};
void to_node(fkyaml::node &n, const LeverDefinition &lever);
void from_node(const fkyaml::node &n, LeverDefinition &lever);
void to_node(fkyaml::node &n, const LeverDependency &dependency);
void from_node(const fkyaml::node &n, LeverDependency &lever);
void to_node(fkyaml::node &n, const InterlockDefinition &dependency);
void from_node(const fkyaml::node &n, InterlockDefinition &lever);
}; // namespace interloq
