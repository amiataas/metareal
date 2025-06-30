#ifndef MR_MRC_PARSER_H
#define MR_MRC_PARSER_H

#include "Lexer.h"

#include <list>
#include <vector>

class Parser {
public:
  Parser(std::list<Token> tokens);
  virtual ~Parser() = default;
  void parse();

private:
  size_t current = 0;
  std::vector<Token> tokens;

  bool eof();
  const TokenKind peek_kind();
  const TokenKind peek_kind(int32_t next);
};

#endif
