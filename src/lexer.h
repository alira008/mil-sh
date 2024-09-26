#pragma once

#include "utils.h"
#include <stddef.h>

enum TokenType {
  TOKEN_INVALID,
  TOKEN_IDENT,
  TOKEN_FLOAT,
  TOKEN_INTEGER,
  TOKEN_ECHO,
  TOKEN_EXIT,
  TOKEN_FLAG_SINGLE_DASH,
  TOKEN_FLAG_DOUBLE_DASH,
  TOKEN_EOF,
};

typedef struct {
  enum TokenType token_type;
  union {
    StringView string;
    float f32;
    int i32;
  } literal;
} Token;

typedef struct {
  Token* data;
  size_t count;
  size_t capacity;
} Tokens;

typedef struct {
  StringView input;
  char ch;
  size_t cur_pos;
  size_t read_pos;
} Lexer;

Lexer lexer_new(StringView);
Token lexer_next_token(Lexer *lexer);
void lexer_print_token(const Token *token);
