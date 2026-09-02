#include "interloq/file.hxx"

void interloq::to_node(fkyaml::node &n, const InterlockDefinition &interlock) {
  n = {
      {"name", interlock.name},
      {"leverframe", interlock.leverframe},
  };
}
void interloq::from_node(const fkyaml::node &n,
                         InterlockDefinition &interlock) {
  if (!n.is_mapping()) {
    throw std::invalid_argument("Expected mapping from YAML import.");
  }
  interlock.name = n["name"].get_value<std::string>();
  interlock.leverframe = {};

  for (const auto &entry :
       n["leverframe"].get_value<std::vector<LeverDefinition>>()) {
    interlock.leverframe.insert({entry.id, entry});
  }
}
void interloq::from_node(const fkyaml::node &n, LeverDefinition &lever) {
  const std::string lever_type_{n["type"].get_value<std::string>()};
  lever.type = magic_enum::enum_cast<LeverType>(lever_type_).value();
  lever.id = n["id"].get_value<unsigned int>();
  lever.name = n["name"].get_value<std::string>();
  if (n.contains("dependencies")) {
    auto dependencies =
        n["dependencies"].get_value<std::vector<LeverDependency>>();
    for (const auto &dependency : dependencies) {
      lever.dependencies.insert({dependency.target, dependency});
    }
  }
}
void interloq::to_node(fkyaml::node &n, const LeverDependency &dependency) {
  n = {{"target", dependency.target},
       {"required_state", dependency.required_state}};
  if (!dependency.alt_choices.empty()) {
    n["alt_choices"] = dependency.alt_choices;
  }
}
void interloq::from_node(const fkyaml::node &n, LeverDependency &dependency) {
  dependency.target = n["target"].get_value<unsigned int>();
  dependency.required_state = n["required_state"].get_value<bool>();
  if (n.contains("alt_choices")) {
    dependency.alt_choices =
        n["alt_choices"].get_value<std::vector<unsigned int>>();
  }
}
void interloq::to_node(fkyaml::node &n, const LeverDefinition &definition) {
  std::string sig_type_str_{magic_enum::enum_name(definition.type)};
  n = {
      {"id", definition.id},
      {"type", sig_type_str_},
      {"name", definition.name},
  };

  if (!definition.dependencies.empty()) {
    std::vector<LeverDependency> map_values_;
    for (const auto &pair : definition.dependencies) {
      map_values_.push_back(pair.second);
    }
    n["dependencies"] = map_values_;
  }
}

interloq::InterlockDefinition
interloq::InterlockDefinition::from_node(const fkyaml::node &data_node) {
  InterlockDefinition output_;
  interloq::from_node(data_node, output_);
  return output_;
}

interloq::InterlockDefinition interloq::InterlockDefinition::yaml_load(
    const std::filesystem::path &input_file) {
  std::ifstream infile_{input_file};
  const auto input_ = fkyaml::node::deserialize(infile_);
  return InterlockDefinition::from_node(input_);
}
