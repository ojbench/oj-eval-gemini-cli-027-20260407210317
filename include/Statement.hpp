#pragma once

#include <memory>
#include <string>

#include "Expression.hpp"

class Program;
class VarState;

class Statement {
 public:
  explicit Statement(std::string source);
  virtual ~Statement() = default;

  virtual void execute(VarState& state, Program& program) const = 0;

  const std::string& text() const noexcept;

 private:
  std::string source_;
};

class LetStmt : public Statement {
 public:
  LetStmt(std::string source, std::string varName, std::unique_ptr<Expression> expr);
  void execute(VarState& state, Program& program) const override;

 private:
  std::string varName_;
  std::unique_ptr<Expression> expr_;
};

class PrintStmt : public Statement {
 public:
  PrintStmt(std::string source, std::unique_ptr<Expression> expr);
  void execute(VarState& state, Program& program) const override;

 private:
  std::unique_ptr<Expression> expr_;
};

class InputStmt : public Statement {
 public:
  InputStmt(std::string source, std::string varName);
  void execute(VarState& state, Program& program) const override;

 private:
  std::string varName_;
};

class GotoStmt : public Statement {
 public:
  GotoStmt(std::string source, int targetLine);
  void execute(VarState& state, Program& program) const override;

 private:
  int targetLine_;
};

class IfStmt : public Statement {
 public:
  IfStmt(std::string source, std::unique_ptr<Expression> leftExpr, char op, std::unique_ptr<Expression> rightExpr, int targetLine);
  void execute(VarState& state, Program& program) const override;

 private:
  std::unique_ptr<Expression> leftExpr_;
  char op_;
  std::unique_ptr<Expression> rightExpr_;
  int targetLine_;
};

class RemStmt : public Statement {
 public:
  RemStmt(std::string source);
  void execute(VarState& state, Program& program) const override;
};

class EndStmt : public Statement {
 public:
  EndStmt(std::string source);
  void execute(VarState& state, Program& program) const override;
};

class IndentStmt : public Statement {
 public:
  IndentStmt(std::string source);
  void execute(VarState& state, Program& program) const override;
};

class DedentStmt : public Statement {
 public:
  DedentStmt(std::string source);
  void execute(VarState& state, Program& program) const override;
};
