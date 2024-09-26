#pragma once

#include "utils.h"
#include <stddef.h>

enum TokenType { TOKEN_IDENT, TOKEN_ECHO, TOKEN_EXIT };

typedef struct {
  enum TokenType token_type;
  union {
    StringView string;
  } literal;
} Token;

typedef struct {
  StringView input;
  char ch;
  size_t cur_pos;
  size_t read_pos;
} Lexer;

Lexer lexer_new(StringView);
Token lexer_next_token(Lexer *lexer);
