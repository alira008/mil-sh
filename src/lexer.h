#pragma once

#include "utils.h"
#include <stddef.h>

enum TokenType {
    TOKEN_ECHO
};

typedef struct {
    enum TokenType token_type;
} Token;

typedef struct {
    StringView input;
    char ch;
    size_t cur_pos;
    size_t read_pos;
} Lexer;

static Lexer lexer_new(StringView);
Token lexer_next_token(Lexer *lexer);
