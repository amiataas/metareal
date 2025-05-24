#include <llvm/Config/llvm-config.h>
#include <stdio.h>
#include "Lexer.h"

int main(int argc, char * argv[]) {
	printf("Metareal compiler using LLVM version: %s\n", LLVM_VERSION_STRING);

	std::unique_ptr<Lexer> lexer = Lexer::from_file("hello.mr");
	lexer->lex();
	return 0;
}
