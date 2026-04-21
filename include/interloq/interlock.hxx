#pragma once
#include <filesystem>
#include <map>

#include "file.hxx"

namespace interloq {
using InterlockSetting = std::map<unsigned int, bool>;
using InterlockPreset = std::map<bool, InterlockSetting>;

class Interlocking {
private:
  const InterlockDefinition logic_table_;
  InterlockSetting lever_states_;

public:
  Interlocking(const std::filesystem::path &def_file)
      : logic_table_(InterlockDefinition::yaml_load(def_file)) {
    for (const auto &[lever, _] : logic_table_.leverframe) {
      lever_states_.insert({lever, false});
    }
  }
  bool move_lever(const unsigned int id, bool state);
};
}; // namespace interloq
