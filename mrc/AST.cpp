#include "AST.h"

#include <memory>

std::unique_ptr<llvm::Value> LiteralExprAST::codegen() {
	return nullptr;
};
