#include "Statement.hpp"

#include <iostream>
#include <limits>
#include <sstream>
#include <utility>

#include "Program.hpp"
#include "VarState.hpp"
#include "utils/Error.hpp"

Statement::Statement(std::string source) : source_(std::move(source)) {}

const std::string& Statement::text() const noexcept { return source_; }

LetStmt::LetStmt(std::string source, std::string varName, std::unique_ptr<Expression> expr)
    : Statement(std::move(source)), varName_(std::move(varName)), expr_(std::move(expr)) {}

void LetStmt::execute(VarState& state, Program& program) const {
  state.setValue(varName_, expr_->evaluate(state));
}

PrintStmt::PrintStmt(std::string source, std::unique_ptr<Expression> expr)
    : Statement(std::move(source)), expr_(std::move(expr)) {}

void PrintStmt::execute(VarState& state, Program& program) const {
  std::cout << expr_->evaluate(state) << "\n";
}

InputStmt::InputStmt(std::string source, std::string varName)
    : Statement(std::move(source)), varName_(std::move(varName)) {}

void InputStmt::execute(VarState& state, Program& program) const {
  int value;
  std::string input;
  while (true) {
    std::cout << " ? ";
    if (!std::getline(std::cin, input)) {
      throw BasicError("EOF");
    }
    try {
      size_t pos;
      value = std::stoi(input, &pos);
      if (pos != input.length()) {
        std::cout << "INVALID NUMBER\n";
        continue;
      }
      break;
    } catch (...) {
      std::cout << "INVALID NUMBER\n";
    }
  }
  state.setValue(varName_, value);
}

GotoStmt::GotoStmt(std::string source, int targetLine)
    : Statement(std::move(source)), targetLine_(targetLine) {}

void GotoStmt::execute(VarState& state, Program& program) const {
  program.changePC(targetLine_);
}

IfStmt::IfStmt(std::string source, std::unique_ptr<Expression> leftExpr, char op, std::unique_ptr<Expression> rightExpr, int targetLine)
    : Statement(std::move(source)), leftExpr_(std::move(leftExpr)), op_(op), rightExpr_(std::move(rightExpr)), targetLine_(targetLine) {}

void IfStmt::execute(VarState& state, Program& program) const {
  int leftVal = leftExpr_->evaluate(state);
  int rightVal = rightExpr_->evaluate(state);
  bool condition = false;
  switch (op_) {
    case '=': condition = (leftVal == rightVal); break;
    case '<': condition = (leftVal < rightVal); break;
    case '>': condition = (leftVal > rightVal); break;
  }
  if (condition) {
    program.changePC(targetLine_);
  }
}

RemStmt::RemStmt(std::string source) : Statement(std::move(source)) {}

void RemStmt::execute(VarState& state, Program& program) const {
  // Do nothing
}

EndStmt::EndStmt(std::string source) : Statement(std::move(source)) {}

void EndStmt::execute(VarState& state, Program& program) const {
  program.programEnd();
}

IndentStmt::IndentStmt(std::string source) : Statement(std::move(source)) {}

void IndentStmt::execute(VarState& state, Program& program) const {
  state.pushScope();
}

DedentStmt::DedentStmt(std::string source) : Statement(std::move(source)) {}

void DedentStmt::execute(VarState& state, Program& program) const {
  state.popScope();
}
