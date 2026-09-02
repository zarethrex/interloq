#pragma once
#include "interloq/file.hxx"
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <map>
#include <stdexcept>
#include <unordered_set>

namespace interloq {
using InterlockSetting = std::map<unsigned int, bool>;

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
  inline bool get_lever_state(unsigned int id) const {
    if (!lever_states_.contains(id)) {
      throw std::range_error("Invalid lever id '" + std::to_string(id) + "'");
    }
    return lever_states_.at(id);
  }
  friend std::ostream &operator<<(std::ostream &os,
                                  const Interlocking &interlock) {
    os << interlock.logic_table_;
    os << "current_state:\n";
    for (const auto &[id, state] : interlock.lever_states_) {
      os << "  " << id << ": " << (state ? "true" : "false") << "\n";
    }
    return os;
  }
};
}; // namespace interloq
