#pragma once

#include "command.h"
#include "lexer.h"
typedef struct {
  Lexer lexer;
  Token peek_token;
} Parser;

Parser parser_new(char *const, size_t);
Commands parser_parse(Parser *parser);
