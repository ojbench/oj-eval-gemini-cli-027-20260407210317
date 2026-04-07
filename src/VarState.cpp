#include "VarState.hpp"

#include <algorithm>

#include "utils/Error.hpp"

VarState::VarState() {
  scopes_.emplace_back();
}

void VarState::setValue(const std::string& name, int value) {
  scopes_.back()[name] = value;
}

int VarState::getValue(const std::string& name) const {
  for (auto it = scopes_.rbegin(); it != scopes_.rend(); ++it) {
    auto found = it->find(name);
    if (found != it->end()) {
      return found->second;
    }
  }
  throw BasicError("VARIABLE NOT DEFINED");
}

void VarState::clear() {
  scopes_.clear();
  scopes_.emplace_back();
}

void VarState::pushScope() {
  scopes_.emplace_back();
}

void VarState::popScope() {
  if (scopes_.size() <= 1) {
    throw BasicError("SCOPE UNDERFLOW");
  }
  scopes_.pop_back();
}

void VarState::resetScopes() {
  while (scopes_.size() > 1) {
    scopes_.pop_back();
  }
}
