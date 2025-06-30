#include "AST.h"
#include "Lexer.h"
#include <llvm/Config/llvm-config.h>
#include <memory>
#include <stdio.h>

int main(int argc, char *argv[]) {
  printf("Metareal compiler using LLVM version: %s\n", LLVM_VERSION_STRING);

  std::unique_ptr<Lexer> lexer = Lexer::from_file("hello.mr");
	std::unique_ptr<Parser> parser = std::make_unique<Parser>(Parser(lexer->lex()));
	parser->parse();
  return 0;
}
