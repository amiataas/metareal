#ifndef MR_MRC_LEXER_H
#define MR_MRC_LEXER_H

#include <fstream>
#include <iosfwd>
#include <list>
#include <memory>
#ifdef __cplusplus

#include <filesystem>
#include <string>

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
};

class Token {
public:
  Token(TokenKind kind);
  Token(TokenKind kind, std::wstring literal);

  std::string to_str() const;

private:
  const TokenKind kind;
  const std::wstring literal;
};

class Lexer {
public:
  Lexer(std::wifstream file);
  ~Lexer();

  void lex();

  static std::unique_ptr<Lexer> from_file(fs::path path);

private:
  std::wifstream _fd;
  std::wstreampos _fsize;
  std::list<Token> tokens;
  void skip_trivia();
  bool eof() const;
};

#endif

#endif
