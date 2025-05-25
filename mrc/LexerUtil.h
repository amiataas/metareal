#ifndef MR_MRC_LEXER_UTIL_H
#define MR_MRC_LEXER_UTIL_H

#include <cstdint>

class LexerUtil {
public:
  const static bool is_whitespace(uint32_t code_point) {
    return (code_point == 0x09 || // HT
            code_point == 0x0B || // VT
            code_point == 0x1F || // US
            code_point == 0x20 || // SP
            code_point == 0x0C || // FF
            code_point == 0xA0    // NBSP
    );
  }
  const static bool is_linefeed(uint32_t code_point) {
    return (code_point == 0x0A ||   // LF
            code_point == 0x0D ||   // CR
            code_point == 0x2028 || // LS
            code_point == 0x2029    // PS
    );
  }
};

#endif
