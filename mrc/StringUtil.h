#ifndef MR_MRC_STRINGUTIL_H
#define MR_MRC_STRINGUTIL_H

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

class StringUtil {
public:
  static const std::string escape_string(std::string input) {
    std::string result;
    for (char c : input) {
      if (c == '\n') {
        result += "\\n";
      } else if (c == '\b') {
        result += "\\b";
      } else if (c == '\r') {
        result += "\\r";
      } else {
        result += c;
      }
    }
    return result;
  }

  static const std::string encode_utf8(uint32_t codepoint) {
    std::string result = "";

    if (codepoint <= 0x7F) {
      result += static_cast<char>(codepoint);
    } else if (codepoint <= 0x7FF) {
      result += static_cast<char>(0xC0 | (codepoint >> 6));
      result += static_cast<char>(0x80 | (codepoint & 0x3F));
    } else if (codepoint <= 0xFFFF) {
      result += static_cast<char>(0xE0 | (codepoint >> 12));
      result += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
      result += static_cast<char>(0x80 | (codepoint & 0x3F));
    } else if (codepoint <= 0x10FFFF) {
      result += static_cast<char>(0xF0 | (codepoint >> 18));
      result += static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F));
      result += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
      result += static_cast<char>(0x80 | (codepoint & 0x3F));
    }

    return result;
  }

  static const uint32_t utf8_from_file(std::ifstream &file, uint8_t first) {
    int num_bytes = utf8_char_length(first);
    uint32_t code_point = 0;

    if ((first & 0b10000000) == 0) {
      num_bytes = 1;
      code_point = first;
    } else if ((first & 0b11100000) == 0b11000000) {
      num_bytes = 2;
      code_point = first & 0b00011111;
    } else if ((first & 0b11110000) == 0b11100000) {
      num_bytes = 3;
      code_point = first & 0b00001111;
    } else if ((first & 0b11111000) == 0b11110000) {
      num_bytes = 4;
      code_point = first & 0b00000111;
    } else {
      return code_point;
    }

    for (int i = 1; i < num_bytes; ++i) {
      int next = file.get();
      if (next == EOF) {
        std::cerr << "Unexpected EOF during UTF-8 decoding\n";
        return code_point;
      }
      unsigned char c = static_cast<unsigned char>(next);
      if ((c & 0b11000000) != 0b10000000) {
        std::cerr << "Invalid UTF-8 continuation byte: 0x" << std::hex << (int)c
                  << std::dec << "\n";
        return code_point;
      }
      code_point = (code_point << 6) | (c & 0b00111111);
    }

    return code_point;
  }

private:
  static const int utf8_char_length(uint8_t byte) {
    if ((byte & 0b10000000) == 0b00000000)
      return 1; // 0xxxxxxx
    if ((byte & 0b11100000) == 0b11000000)
      return 2; // 110xxxxx
    if ((byte & 0b11110000) == 0b11100000)
      return 3; // 1110xxxx
    if ((byte & 0b11111000) == 0b11110000)
      return 4; // 11110xxx
    return -1;  // Invalid UTF-8 start byte
  }
};

#endif
