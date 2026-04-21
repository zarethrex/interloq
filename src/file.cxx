#include "interloq/file.hxx"

void interloq::to_node(fkyaml::node& n, const InterlockDefinition& interlock) {
    n = {
        {"name", interlock.name},
       {"leverframe",  interlock.leverframe},
    };
}
void interloq::from_node(const fkyaml::node& n, InterlockDefinition& interlock) {
    if(!n.is_mapping()) {
        throw std::invalid_argument("Expected mapping from YAML import.");
    }
    interlock.name = n["name"].get_value<std::string>();
    interlock.leverframe = {};

    for(const auto& entry : n["leverframe"].get_value<std::vector<LeverDefinition>>()) {
        interlock.leverframe.insert({entry.id, entry});
    }
}
void interloq::from_node(const fkyaml::node& n, LeverDefinition& lever) {
    const std::string lever_type_{n["type"].get_value<std::string>()};
    lever.type = magic_enum::enum_cast<LeverType>(lever_type_).value();
    lever.id = n["id"].get_value<unsigned int>();
    lever.name = n["name"].get_value<std::string>();
    if(!n.contains("dependencies")) {
        lever.dependencies = std::nullopt;
    } else {
        lever.dependencies = n["dependencies"].get_value<std::vector<LeverDependency>>();
    }
}
void interloq::to_node(fkyaml::node& n, const LeverDependency& dependency) {
    n = {
        {"target", dependency.target},
        {"required_state", dependency.required_state}
    };
}
void interloq::from_node(const fkyaml::node& n, LeverDependency& dependency) {
    dependency.target = n["target"].get_value<unsigned int>();
    dependency.required_state = n["required_state"].get_value<bool>();
}
void interloq::to_node(fkyaml::node& n, const LeverDefinition& definition) {
    std::string sig_type_str_{magic_enum::enum_name(definition.type)};
    n = {
        {"id", definition.id},
        {"type", sig_type_str_},
        {"name", definition.name},
    };

    if(definition.dependencies.has_value()) {
        n["dependencies"] = definition.dependencies.value();
    }
}

interloq::InterlockDefinition interloq::InterlockDefinition::yaml_load(const std::filesystem::path& input_file) {
    std::ifstream infile_{input_file};
    const auto input_ = fkyaml::node::deserialize(infile_);
    InterlockDefinition output_;
    interloq::from_node(input_, output_);
    return output_;
}