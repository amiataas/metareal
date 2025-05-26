#ifndef MR_MRC_LEXER_UTIL_H
#define MR_MRC_LEXER_UTIL_H

#include <cstdint>

class LexerUtil {
public:
  const static inline bool is_whitespace(uint32_t code_point) {
    return (code_point == 0x09 || // HT
            code_point == 0x0B || // VT
            code_point == 0x1F || // US
            code_point == 0x20 || // SP
            code_point == 0x0C || // FF
            code_point == 0xA0    // NBSP
    );
  }
  const static inline bool is_linefeed(uint32_t code_point) {
    return (code_point == 0x0A ||   // LF
            code_point == 0x0D ||   // CR
            code_point == 0x2028 || // LS
            code_point == 0x2029    // PS
    );
  }

  const static inline bool is_digit(uint32_t code_point) {
    return (0x2F < code_point && 0x3A > code_point); // 0-9
  }

  const static inline bool is_hex_digit(uint32_t code_point) {
    return is_digit(code_point) ||
           (0x40 < code_point && 0x47 > code_point) || // A-F
           (0x60 < code_point && 0x67 > code_point);   // a-f
  }
};

#endif
