
#include "Lexer.h"
#include "LexerUtil.h"
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <ios>
#include <iostream>
#include <memory>
#include <string>

Token::Token(TokenKind kind) : kind(kind), literal({}) {};
Token::Token(TokenKind kind, std::wstring literal)
    : kind(kind), literal(literal) {};

#define CASE(KIND, STR)                                                        \
  case TokenKind::KIND:                                                        \
    return std::string(u8##STR)

std::string Token::to_str() const {
  switch (this->kind) {
    CASE(Plus, "<+>");
    CASE(Minus, "<->");
    CASE(Asterisk, "<\u00D7>");
    CASE(Slash, "<\u00F7>");
    CASE(Percent, "<%>");
    CASE(Amp, "<&>");
    CASE(Pipe, "<|>");
    CASE(Equal, "<=>");
    CASE(Exclam, "<!>");

    CASE(PlusPlus, "<++>");
    CASE(MinusMinus, "<-->");
    CASE(AmpAmp, "<\u2227>");
    CASE(PipePipe, "<\u2228>");

    CASE(PlusEqual, "<=,+>");
    CASE(MinusEqual, "<=,->");
    CASE(AsteriskEqual, "<=,\u00D7>");
    CASE(SlashEqual, "<=,\u00F7>");
    CASE(PercentEqual, "<=,%>");

    CASE(EqualEqual, "<\u2261>");
    CASE(ExclamEqual, "<\u2260>");

    CASE(EqualBig, "<\u227B>");
    CASE(Eof, "<eof>");
  default:
    return std::string(u8"");
  }
}
#undef CASE

std::unique_ptr<Lexer> Lexer::from_file(fs::path path) {
  return std::make_unique<Lexer>(std::wifstream(path));
}

Lexer::Lexer(std::wifstream file) {
  this->_fd = std::move(file);
  if (!this->_fd.is_open()) {
    std::cerr << "Error opening the file!";
    return;
  }

  this->_fd.seekg(0, std::ios::end);
  this->_fsize = this->_fd.tellg();
  this->_fd.seekg(0, std::ios::beg);
}

Lexer::~Lexer() {
  if (this->_fd.is_open())
    this->_fd.close();
}

bool Lexer::eof() const { return this->_fd.eof(); }

#define ADVANCE(TOKEN)                                                         \
  this->tokens.push_back(Token(TokenKind::TOKEN));                             \
  this->_fd.seekg(1, std::ios::cur)

void Lexer::lex() {
  this->tokens = std::list<Token>();

  std::wstreampos start = this->_fd.tellg();

  while (!eof()) {
    this->skip_trivia();

    switch (this->_fd.get()) {
    case '(': {
      this->tokens.push_back(Token(TokenKind::LParen));
      break;
    }
    case ')': {
      this->tokens.push_back(Token(TokenKind::RParen));
      break;
    }
    case '[': {
      this->tokens.push_back(Token(TokenKind::LBrak));
      break;
    }
    case ']': {
      this->tokens.push_back(Token(TokenKind::RBrak));
      break;
    }
    case '{': {
      this->tokens.push_back(Token(TokenKind::LBrace));
      break;
    }
    case '}': {
      this->tokens.push_back(Token(TokenKind::RBrace));
      break;
    }
    case '.': {
      this->tokens.push_back(Token(TokenKind::Dot));
      break;
    }
    case ',': {
      this->tokens.push_back(Token(TokenKind::Comma));
      break;
    }
    case ':': {
      this->tokens.push_back(Token(TokenKind::Colon));
      break;
    }
    case ';': {
      this->tokens.push_back(Token(TokenKind::Semicolon));
      break;
    }
    case '^': {
      this->tokens.push_back(Token(TokenKind::Caret));
      break;
    }
    case '~': {
      this->tokens.push_back(Token(TokenKind::Tilde));
      break;
    }
    case '+': {
      const uint32_t next = this->_fd.peek();
      if (next == '+') {
        ADVANCE(PlusPlus);
        break;
      } else if (next == '=') {
        ADVANCE(PlusEqual);
        break;
      }
      this->tokens.push_back(Token(TokenKind::Plus));
      break;
    }
    case '-': {
      const uint32_t next = this->_fd.peek();
      if (next == '-') {
        ADVANCE(MinusMinus);
        break;
      } else if (next == '=') {
        ADVANCE(MinusEqual);
        break;
      } else if (next == '>') {
        ADVANCE(Arrow);
        break;
      }
      this->tokens.push_back(Token(TokenKind::Minus));
      break;
    }
    case '*': {
      const uint32_t next = this->_fd.peek();
      if (next == '=') {
        ADVANCE(AsteriskEqual);
        break;
      }
      this->tokens.push_back(Token(TokenKind::Asterisk));
      break;
    }
    case '/': {
      const uint32_t next = this->_fd.peek();
      if (next == '=') {
        ADVANCE(SlashEqual);
        break;
      }
      this->tokens.push_back(Token(TokenKind::Slash));
      break;
    }
    case '%': {
      const uint32_t next = this->_fd.peek();
      if (next == '=') {
        ADVANCE(PercentEqual);
        break;
      }
      this->tokens.push_back(Token(TokenKind::Percent));
      break;
    }
    case '&': {
      const uint32_t next = this->_fd.peek();
      if (next == '=') {
        ADVANCE(AmpEqual);
        break;
      } else if (next == '&') {
        ADVANCE(AmpAmp);
        break;
      }
      this->tokens.push_back(Token(TokenKind::Amp));
      break;
    }
    case '|': {
      const uint32_t next = this->_fd.peek();
      if (next == '=') {
        ADVANCE(PipeEqual);
        break;
      } else if (next == '|') {
        ADVANCE(PipePipe);
        break;
      }
      this->tokens.push_back(Token(TokenKind::Pipe));
      break;
    }
    case '=': {
      const uint32_t next = this->_fd.peek();
      if (next == '=') {
        ADVANCE(EqualEqual);
        break;
      } else if (next == '>') {
        ADVANCE(EqualBig);
        break;
      }

      this->tokens.push_back(Token(TokenKind::Equal));
      break;
    }
    case '!': {
      const uint32_t next = this->_fd.peek();
      if (next == '=') {
        ADVANCE(ExclamEqual);
        break;
      }
      this->tokens.push_back(Token(TokenKind::Exclam));
      break;
    }
    case '<': {
      const uint32_t next = this->_fd.peek();
      if (next == '=') {
        ADVANCE(LesserEqual);
        break;
      } else if (next == '<') {
        this->_fd.seekg(1, std::ios::cur);
        if (this->_fd.peek() == '=') {
          ADVANCE(LesserLesserEqual);
          break;
        } else {
          this->_fd.seekg(-1, std::ios::cur);
          ADVANCE(LesserLesser);
          break;
        }
      }
    }
    case '>': {
      const uint32_t next = this->_fd.peek();
      if (next == '=') {
        ADVANCE(GreaterEqual);
        break;
      } else if (next == '>') {
        this->_fd.seekg(1, std::ios::cur);
        if (this->_fd.peek() == '=') {
          ADVANCE(GreaterGreaterEqual);
          break;
        } else {
          this->_fd.seekg(-1, std::ios::cur);
          ADVANCE(GreaterGreater);
          break;
        }
      }
      this->tokens.push_back(Token(TokenKind::Greater));
      break;
    }
    case (unsigned int)EOF:
      this->tokens.push_back(Token(TokenKind::Eof));
      goto finalize;
      break;
    }
  }
finalize:

  for (auto v : this->tokens)
    std::cout << v.to_str() << " ";
  std::cout << "\n";
}

#undef ADVANCE

void Lexer::skip_trivia() {
  uint32_t current = 0;
  while (!this->eof()) {
    current = this->_fd.peek();
    if (LexerUtil::is_whitespace(current)) {
      this->_fd.seekg(1, std::ios::cur);
      continue;
    } else if (current == '/') {
      this->_fd.seekg(1, std::ios::cur);
      current = this->_fd.peek();
      if (current == '/') {
        this->_fd.get();
        while (!eof() && !LexerUtil::is_linefeed(this->_fd.get())) {
        }
        continue;
      } else if (current == '*') {
        while (!eof()) {
          current = this->_fd.get();
          if (current == '*' && this->_fd.get() == '/') {
            this->_fd.seekg(1, std::ios::cur);
            break;
          }
        }
      }
      this->_fd.seekg(-1, std::ios::cur);
      break;
    } else {
      break;
    }
  }
}
