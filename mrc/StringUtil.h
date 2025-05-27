#ifndef MR_MRC_STRINGUTIL_H
#define MR_MRC_STRINGUTIL_H

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
};

#endif
