#pragma once

#include "lexer.h"
typedef struct {
  Lexer lexer;
  Tokens tokens;
} Parser;

Parser parser_new(char *const, size_t);
void parser_parse(Parser *parser);
void parser_free_tokens(Parser*);
