#ifndef MR_MRC_LEXER_H
#define MR_MRC_LEXER_H

#include "llvm/ADT/StringRef.h"
#ifdef __cplusplus

#include <filesystem>
#include <fstream>
#include <iosfwd>
#include <list>
#include <memory>
#include <string>
#include <unordered_map>

namespace fs = std::filesystem;

enum class TokenKind {
  Eof = -1,
  Plus = 1,
  Minus,
  Asterisk,
  Slash,
  Percent,
  Pipe,
  Amp,
  Equal,
  Exclam,
  Tilde,
  Caret,

  LParen,
  RParen,
  LBrak,
  RBrak,
  LBrace,
  RBrace,

  Dot,
  Comma,
  Colon,
  Semicolon,

  PlusPlus,
  MinusMinus,
  AmpAmp,
  PipePipe,

  Lesser,
  LesserEqual,
  LesserLesser,
  Greater,
  GreaterEqual,
  GreaterGreater,

  PlusEqual,
  MinusEqual,
  AsteriskEqual,
  SlashEqual,
  PercentEqual,
  AmpEqual,
  PipeEqual,
  EqualEqual,
  ExclamEqual,

  GreaterGreaterEqual,
  LesserLesserEqual,

  Arrow,
  EqualBig,

  Numeric,
  String,
  Identifier,

  True,
  False,
  Let,
  Type,
  Func,
};

enum class LexerErrorCode {
  NoError = 0,
  InvalidHexNumericLiteral,
  IncompleteExponentLiteral,

  UnterminatedString,
  UnterminatedHexByte,
  UnterminatedUnicodeCharacter,
};

class Token {
public:
  Token(TokenKind kind);
  Token(TokenKind kind, std::string literal);

  std::string to_str() const;
	llvm::StringRef to_strref() const;
  const TokenKind kind;
  const std::string literal;

private:
};

class Lexer {
public:
  Lexer(std::ifstream file);
  ~Lexer();

  std::list<Token> lex();

  static std::unique_ptr<Lexer> from_file(fs::path path);

private:
  std::ifstream _fd;
  std::streampos _fsize;
  std::list<Token> tokens;
  LexerErrorCode errorCode = LexerErrorCode::NoError;

  std::string lex_numeric(uint32_t start);
  std::string lex_string(uint32_t start);
  void skip_trivia();
  bool eof() const;

  const static std::unordered_map<std::string, TokenKind> Keywords;
};

#endif

#endif
