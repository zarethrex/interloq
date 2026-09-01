#include "interloq/interlock.hxx"

bool interloq::Interlocking::move_lever(unsigned int id, bool state) {
  if (!lever_states_.contains(id)) {
    throw std::invalid_argument("Lever frame contains no lever of ID '" +
                                std::to_string(id) + "'");
  }
  const LeverDefinition entry_{logic_table_.leverframe.at(id)};

  std::vector<unsigned int> processed_;

  for (const auto &[target, dependency] : entry_.dependencies) {
    if (std::find(processed_.begin(), processed_.end(), target) !=
        processed_.end()) {
      continue;
    }
    std::vector<unsigned int> test_cases_{target};
    test_cases_.insert(test_cases_.begin(), dependency.alt_choices.begin(),
                       dependency.alt_choices.end());
    processed_.insert(processed_.begin(), test_cases_.begin(),
                      test_cases_.end());
    bool dependency_passed_{false};
    for (const unsigned int test_id : test_cases_) {
      if (lever_states_.at(test_id) ==
          entry_.dependencies.at(test_id).required_state) {
        dependency_passed_ = true;
        break;
      }
    }
    if (!dependency_passed_) {
      return false;
    }
  }
  lever_states_[id] = state;

  return true;
}
