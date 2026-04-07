#include "Program.hpp"
#include "Statement.hpp"
#include "utils/Error.hpp"

Program::Program() : programCounter_(-1), programEnd_(false) {}

void Program::addStmt(int line, std::unique_ptr<Statement> stmt) {
  recorder_.add(line, std::move(stmt));
}

void Program::removeStmt(int line) {
  recorder_.remove(line);
}

void Program::run() {
  resetBeforeRun();
  programCounter_ = recorder_.nextLine(-1);
  while (programCounter_ != -1 && !programEnd_) {
    const Statement* stmt = recorder_.get(programCounter_);
    if (!stmt) {
      break;
    }
    int currentPC = programCounter_;
    stmt->execute(vars_, *this);
    if (programCounter_ == currentPC) {
      programCounter_ = recorder_.nextLine(programCounter_);
    } else {
      if (programCounter_ != -1 && !recorder_.hasLine(programCounter_)) {
        throw BasicError("LINE NUMBER ERROR");
      }
    }
  }
}

void Program::list() const {
  recorder_.printLines();
}

void Program::clear() {
  recorder_.clear();
  vars_.clear();
  resetBeforeRun();
}

void Program::execute(Statement* stmt) {
  stmt->execute(vars_, *this);
}

int Program::getPC() const noexcept {
  return programCounter_;
}

void Program::changePC(int line) {
  programCounter_ = line;
}

void Program::programEnd() {
  programEnd_ = true;
}

void Program::resetBeforeRun() noexcept {
  programCounter_ = -1;
  programEnd_ = false;
  vars_.resetScopes();
}