
#include "Lexer.h"
#include <cstdio>
#include <fstream>
#include <ios>
#include <iostream>
#include <memory>
#include <string>

Token::Token(TokenKind kind) : kind(kind), literal({}) {};
Token::Token(TokenKind kind, std::wstring literal)
    : kind(kind), literal(literal) {};

std::wstring Token::to_str() const {
  switch (this->kind) {
  default:
    return std::wstring();
  }
}

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

void Lexer::lex() {
  this->tokens = std::list<Token>();

  std::wstreampos start = this->_fd.tellg();

  while (!eof()) {
    if (this->_fd.get() == EOF) {
      this->tokens.push_back(Token(TokenKind::Eof));
      break;
    }
  }

  for (auto v : this->tokens)
    std::wcout << v.to_str() << "\n";
}
