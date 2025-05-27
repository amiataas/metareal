#ifndef MR_MRC_STRINGUTIL_H
#define MR_MRC_STRINGUTIL_H

#include <cstdint>
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
};

#endif
