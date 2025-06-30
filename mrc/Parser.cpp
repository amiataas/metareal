#include "Parser.h"

Parser::Parser(std::list<Token> tokens) {
  this->tokens = std::vector(tokens.begin(), tokens.end());
}

bool Parser::eof() {
  return this->tokens[this->current].kind == TokenKind::Eof;
}

const TokenKind Parser::peek_kind() { return this->tokens[this->current].kind; }

const TokenKind Parser::peek_kind(int32_t next) {
  return this->tokens[this->current + next].kind;
}

void Parser::parse() {
  if (this->tokens.size() == 0) {
    return;
  }
  Token token = this->tokens[this->current];

  while (!this->eof()) {
    const TokenKind ctk = this->peek_kind();

		switch (ctk) {

		};
  }
}
