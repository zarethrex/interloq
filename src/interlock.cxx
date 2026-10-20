#include "interloq/interlock.hxx"

bool interloq::Interlocking::lever_locked(const unsigned int id) const {
  if (!lever_states_.contains(id)) {
    throw std::invalid_argument("Lever frame contains no lever of ID '" +
                                std::to_string(id) + "'");
  }
  bool locked_{false};
  const LeverDefinition &entry_{logic_table_.leverframe.at(id)};

  std::unordered_set<unsigned int> processed_;

  for (const auto &[target, dependency] : entry_.dependencies) {
    if (processed_.contains(target)) {
      continue;
    }

    bool dependency_passed_{true};

    // Check if target lever has the required state
    if (lever_states_.at(target) != dependency.required_state) {
      dependency_passed_ = false;
      // Check alternative choices
      for (const unsigned int alt_id : dependency.alt_choices) {
        if (!entry_.dependencies.contains(alt_id)) {
          throw std::range_error("Specified 'alt_choices' value for lever " +
                                 std::to_string(id) +
                                 " is not a recognised dependency.");
        }
        if (lever_states_.at(alt_id) ==
            entry_.dependencies.at(alt_id).required_state) {
          dependency_passed_ = true;
          break;
        }
      }
      locked_ = !dependency_passed_;
    }

    // Mark all checked levers as processed to avoid duplicate checks
    processed_.insert(target);
    processed_.insert(dependency.alt_choices.begin(),
                      dependency.alt_choices.end());

    if (!dependency_passed_) {
      return locked_;
    }
  }
  return locked_;
}

bool interloq::Interlocking::move_lever(unsigned int id, bool state) {
  if (!lever_locked(id)) {
    lever_states_[id] = state;
    return true;
  }
  return false;
}
