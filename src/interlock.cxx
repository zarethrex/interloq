#include "interloq/interlock.hxx"
#include "interloq/file.hxx"
#include <stdexcept>

bool interloq::Interlocking::move_lever(unsigned int id, bool state) {
  if (!lever_states_.contains(id)) {
    throw std::invalid_argument("Lever frame contains no lever of ID '" +
                                std::to_string(id) + "'");
  }
  const LeverDefinition entry_{logic_table_.leverframe.at(id)};
  if (entry_.dependencies.has_value()) {
    for (const auto &[id, state] : entry_.dependencies.value()) {
      if (lever_states_.at(id) != state) {
        return false;
      }
    }
  }
  lever_states_[id] = state;
  return true;
}