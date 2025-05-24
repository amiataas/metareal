#ifndef MR_MRC_LEXER_H
#define MR_MRC_LEXER_H


#include <fstream>
#include <iosfwd>
#include <list>
#include <memory>
#ifdef __cplusplus

#include <string>
#include <filesystem>

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
  Equal
};

class Token {
public:
  Token(TokenKind kind);
  Token(TokenKind kind, std::wstring literal);

	std::wstring to_str() const;

private:
  const TokenKind kind;
  const std::wstring literal;
};

class Lexer {
	public:
		Lexer(std::wifstream file);
		~Lexer();

		void lex();

		static std::unique_ptr<Lexer> from_file( fs::path path );

	private:
		std::wifstream _fd;
		std::wstreampos _fsize;
		std::list<Token> tokens;
		bool eof() const;
};

#endif

#endif
