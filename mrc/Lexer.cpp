#include "Lexer.h"
#include "LexerUtil.h"
#include "StringUtil.h"
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <ios>
#include <iostream>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>

Token::Token(TokenKind kind) : kind(kind), literal({}) {};
Token::Token(TokenKind kind, std::string literal)
    : kind(kind), literal(literal) {};

#define CASE(KIND, STR)                                                        \
  case TokenKind::KIND:                                                        \
    return std::string(u8##STR)

std::string Token::to_str() const {
  switch (this->kind) {
    CASE(Plus, "<'+'>");
    CASE(Minus, "<'-'>");
    CASE(Asterisk, "<'\u00D7'>");
    CASE(Slash, "<'\u00F7'>");
    CASE(Percent, "<'%'>");
    CASE(Amp, "<'&'>");
    CASE(Pipe, "<'|'>");
    CASE(Equal, "<'='>");
    CASE(Exclam, "<'!'>");
    CASE(Tilde, "<'~'>");
    CASE(Caret, "<'^'>");

    CASE(LParen, "<'('>");
    CASE(RParen, "<')'>");
    CASE(LBrak, "<'['>");
    CASE(RBrak, "<']'>");
    CASE(LBrace, "<'{'>");
    CASE(RBrace, "<'}'>");

    CASE(Dot, "<'.'>");
    CASE(Comma, "<','>");
    CASE(Colon, "<':'>");
    CASE(Semicolon, "<';'>");

    CASE(PlusPlus, "<'++'>");
    CASE(MinusMinus, "<'--'>");
    CASE(AmpAmp, "<'\u2227'>");
    CASE(PipePipe, "<'\u2228'>");

    CASE(Lesser, "<'<'>");
    CASE(LesserEqual, "<'\u2264'>");
    CASE(LesserLesser, "<'\u226A'>");
    CASE(Greater, "<'>'>");
    CASE(GreaterEqual, "<'\u2265'>");
    CASE(GreaterGreater, "<'\u226B'>");

    CASE(PlusEqual, "<=,'+'>");
    CASE(MinusEqual, "<=,'-'>");
    CASE(AsteriskEqual, "<=,'\u00D7'>");
    CASE(SlashEqual, "<=,'\u00F7'>");
    CASE(PercentEqual, "<=,'%'>");
    CASE(AmpEqual, "<=,'&'>");
    CASE(PipeEqual, "<=,'|'>");
    CASE(EqualEqual, "<'\u2261'>");
    CASE(ExclamEqual, "<'\u2260'>");
    CASE(LesserLesserEqual, "<=,'<<'>");
    CASE(GreaterGreaterEqual, "<=,'>>'>");

    CASE(Arrow, "<'\u2192'>");
    CASE(EqualBig, "<'\u21D2'>");

    CASE(Eof, "<eof>");

  case TokenKind::Numeric: {
    std::string rv = "<num:";
    rv.append(this->literal);
    rv.push_back('>');
    return rv;
  }

  case TokenKind::String: {
    std::string rv = "<str:";
    const std::string escaped = StringUtil::escape_string(this->literal);
    rv.append(escaped);
    rv.push_back('>');
    return rv;
  }

  default:
    return std::string(u8"");
  }
}
#undef CASE

std::unique_ptr<Lexer> Lexer::from_file(fs::path path) {
  return std::make_unique<Lexer>(std::ifstream(path));
}

Lexer::Lexer(std::ifstream file) {
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

  std::streampos start = this->_fd.tellg();

  while (!eof()) {
    this->skip_trivia();

    uint32_t ch = this->_fd.get();

    switch (ch) {
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
      this->tokens.push_back(Token(TokenKind::Lesser));
      break;
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
    default: {
      if (LexerUtil::is_digit(ch)) {
        std::string literal = lex_numeric(ch);
        this->tokens.push_back(Token(TokenKind::Numeric, literal));
      } else if (ch == '\'' || ch == '`' || ch == '"') {
        std::string literal = lex_string(ch);
        this->tokens.push_back(Token(TokenKind::String, literal));
      }
    }
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
        this->_fd.seekg(-1, std::ios::cur);
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
      continue;
    } else {
      break;
    }
  }
}

std::string Lexer::lex_numeric(uint32_t start) {
  std::stringstream literal;
  literal.put(start);

  if (start == '0') {
    if (this->_fd.peek() == 'x') {
      literal.put(this->_fd.get()); // literal has '0x' until now

      if (!eof() && LexerUtil::is_hex_digit(this->_fd.peek())) {
        literal.put(this->_fd.get());

        while (!eof() && LexerUtil::is_hex_digit(this->_fd.peek())) {
          literal.put(this->_fd.get());
        }
      } else {
        this->errorCode = LexerErrorCode::InvalidHexNumericLiteral;
        return literal.str();
      }
    }
  }

  while (!eof() && LexerUtil::is_digit(this->_fd.peek())) {
    literal.put(this->_fd.get());
  }

  if (this->_fd.peek() == '.') {
    literal.put(this->_fd.get());

    while (!eof() && LexerUtil::is_digit(this->_fd.peek())) {
      literal.put(this->_fd.get());
    }

    if (this->_fd.peek() == 'e' || this->_fd.peek() == 'E') {
      literal.put(this->_fd.get());
      if (this->_fd.peek() == '+' || this->_fd.peek() == '-') {
        literal.put(this->_fd.get());

        if (!LexerUtil::is_digit(this->_fd.peek())) {
          this->errorCode = LexerErrorCode::IncompleteExponentLiteral;
        }
      }

      while (!eof() && LexerUtil::is_digit(this->_fd.peek())) {
        literal.put(this->_fd.get());
      }
    }
  }

  return literal.str();
}

std::string Lexer::lex_string(uint32_t start) {
  std::stringstream literal;
  uint32_t current = this->_fd.peek();

  while ((current = this->_fd.get()) != start) {
    if (eof() || LexerUtil::is_linefeed(current)) {
      this->errorCode = LexerErrorCode::UnterminatedString;
      break;
    }

    // Handle Escape Sequences
    if ('\\' == current) {
      current = this->_fd.get();
      switch (current) {
      case '\\':
        literal.put('\\');
        break;
      case '\'':
        if ('\'' == start) {
          literal.put(0x27); // '
        } else {
          literal.put(0x5C); // /
          literal.put(0x27); // '
        }
        break;
      case '"':
        if ('"' == start) {
          literal.put(0x22); // "
        } else {
          literal.put(0x5C); // /
          literal.put(0x22); // "
        }
        break;
      case 'n':
        literal.put(0x0A);
        break;
      case 'r':
        literal.put(0x0D);
        break;
      case 't':
        literal.put(0x09);
        break;
      case 'b':
        literal.put(0x08);
        break;
      case 'f':
        literal.put(0x0C);
        break;
      case 'a':
        literal.put(0x07);
        break;
      case 'v':
        literal.put(0x0B);
        break;
      case '0':
        literal.put(0x00);
        break;
      case 'x': {
        std::stringstream value;
        for (int i = 0; i < 2; ++i) {
          current = this->_fd.get();
          if (LexerUtil::is_hex_digit(current)) {
            value.put(current);
          } else {
            this->errorCode = LexerErrorCode::UnterminatedHexByte;
            return literal.str();
          }
        }
        uint8_t v = static_cast<uint8_t>(std::stoi(value.str(), nullptr, 16));
        literal << StringUtil::encode_utf8(v);
        break;
      }
      case 'u': {
        std::stringstream value;
        for (int i = 0; i < 4; ++i) {
          current = this->_fd.get();
          if (LexerUtil::is_hex_digit(current)) {
            value.put(current);
          } else {
            this->errorCode = LexerErrorCode::UnterminatedUnicodeCharacter;
            return literal.str();
          }
        }
        uint32_t v =
            static_cast<uint32_t>(std::stoul(value.str(), nullptr, 16));
        literal << StringUtil::encode_utf8(v);
        break;
      }
      default:
        if (LexerUtil::is_whitespace(current)) {
          skip_trivia();
          if (!LexerUtil::is_linefeed(this->_fd.get())) {
            this->errorCode = LexerErrorCode::UnterminatedString;
            return literal.str();
          }
        }
      }
      continue;
    }

    if (current != start) {
      literal.put(current);
    }
  }

  return literal.str();
}
