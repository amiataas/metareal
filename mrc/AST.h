#ifndef MR_MRC_AST_H
#define MR_MRC_AST_H

#include "Lexer.h"

#include <memory>

#include <llvm/IR/Value.h>

enum class Operation {
  Add,
  Sub,
};

// a path like @{a.b.c}::InternalExport
class Path {

};

// a path like @{a.b.c}:: 
class InternalPath {

};

class ExternalPath {

};

class Module {

};

class Alias {

};

class Declaration {

};

class Expression {
public:
  virtual ~Expression() = default;
  virtual std::unique_ptr<llvm::Value> codegen() = 0;
};

class LiteralExpression : public Expression {
public:
  LiteralExpression(std::unique_ptr<Token> token) : token(std::move(token)) {}
  std::unique_ptr<llvm::Value> codegen() override;

private:
  std::unique_ptr<Token> token;
};

class BinaryExpr : public Expression {
  Operation op;
  std::unique_ptr<Expression> left, right;

public:
  BinaryExpr(Operation op, std::unique_ptr<Expression> left,
                std::unique_ptr<Expression> right)
      : op(op), left(std::move(left)), right(std::move(right)) {}
};

#endif
