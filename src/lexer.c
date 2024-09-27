#include "lexer.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define LEXER_IS_END (lexer->read_pos > lexer->input.count)
#define KEYWORDS_LEN 2
static String KEYWORDS_STRING[KEYWORDS_LEN] = {STRING_LIT("echo"),
                                               STRING_LIT("exit")};
static enum TokenType KEYWORDS_TOKENTYPE[KEYWORDS_LEN] = {TOKEN_ECHO,
                                                          TOKEN_EXIT};

StringView lexer_read_ident(Lexer *);
bool lexer_read_number(Lexer *, enum TokenType *, int32_t *, float *);
void lexer_read_char(Lexer *);
void lexer_skip_whitespace(Lexer *);
char lexer_peek_char(Lexer *);
StringView lexer_lex_string(Lexer *);
int lexer_find_keyword(StringView);

Lexer lexer_new(StringView input) {
  Lexer lexer = {.input = input, .ch = '\0'};
  lexer_read_char(&lexer);
  return lexer;
}

Token lexer_next_token(Lexer *lexer) {
  lexer_skip_whitespace(lexer);
  Token token = {0};
  if (isalpha(lexer->ch)) {
    StringView string_view = lexer_read_ident(lexer);
    // grab keyword index from list
    int location = lexer_find_keyword(string_view);
    if (location != -1) {
      token.token_type = KEYWORDS_TOKENTYPE[location];
    } else {
      token.token_type = TOKEN_IDENT;
      token.literal.string = string_view;
    }
  } else if (lexer->ch == '-') {
    token.token_type = TOKEN_FLAG_SINGLE_DASH;
    lexer_read_char(lexer);
    if (lexer->ch == '-') {
      token.token_type = TOKEN_FLAG_DOUBLE_DASH;
      lexer_read_char(lexer);
    }
    // read identifier of flag
    if (isalpha(lexer->ch)) {
      token.literal.string = lexer_read_ident(lexer);
    } else {
      token.token_type = TOKEN_INVALID;
    }
  } else if (isdigit(lexer->ch)) {
    int i32 = 0;
    float f32 = 0.0;
    if (!lexer_read_number(lexer, &token.token_type, &i32, &f32)) {
      fprintf(stderr, "could not convert string to number");
    } else {
      switch (token.token_type) {
      case TOKEN_INTEGER:
        token.literal.i32 = i32;
        break;
      case TOKEN_FLOAT:
        token.literal.f32 = f32;
        break;
      default:
        fprintf(stderr, "could not convert string to number");
        break;
      }
    }

  } else if (lexer->ch == '\0') {
    token.token_type = TOKEN_EOF;
  } else {
    token.token_type = TOKEN_INVALID;
  }

  lexer_read_char(lexer);
  return token;
}

void lexer_read_char(Lexer *lexer) {
  if (LEXER_IS_END) {
    lexer->ch = '\0';
  } else {
    lexer->ch = lexer->input.data[lexer->read_pos];
  }

  lexer->cur_pos = lexer->read_pos;
  ++lexer->read_pos;
}

void lexer_skip_whitespace(Lexer *lexer) {
  while (!LEXER_IS_END && isspace(lexer->ch)) {
    lexer_read_char(lexer);
  }
}

char lexer_peek_char(Lexer *lexer) {
  if (LEXER_IS_END)
    return '\0';
  return lexer->input.data[lexer->read_pos];
}

int lexer_find_keyword(StringView identifier) {
  int location = -1;
  for (int i = 0; i < KEYWORDS_LEN; ++i) {
    if (identifier.count != KEYWORDS_STRING[i].count) {
      continue;
    }
    if (strncmp(identifier.data, KEYWORDS_STRING[i].data, identifier.count) ==
        0) {
      location = i;
      break;
    }
  }
  return location;
}

void lexer_print_token(FILE *file, const Token *const token) {
  if (token->token_type == TOKEN_EXIT) {
    fprintf(file, "'exit'");
  } else if (token->token_type == TOKEN_ECHO) {
    fprintf(file, "'echo'");
  } else if (token->token_type == TOKEN_IDENT) {
    fprintf(file, "'%.*s'", (int)token->literal.string.count,
            token->literal.string.data);
  } else if (token->token_type == TOKEN_FLAG_SINGLE_DASH) {
    fprintf(file, "'-%.*s'", (int)token->literal.string.count,
            token->literal.string.data);
  } else if (token->token_type == TOKEN_FLAG_DOUBLE_DASH) {
    fprintf(file, "'--%.*s'", (int)token->literal.string.count,
            token->literal.string.data);
  } else if (token->token_type == TOKEN_INVALID) {
    fprintf(file, "invalid token");
  }
}
bool lexer_read_number(Lexer *lexer, enum TokenType *token_type, int32_t *i32,
                       float *f32) {
  size_t pos = lexer->cur_pos;
  while (!LEXER_IS_END && isdigit(lexer->ch)) {
    lexer_read_char(lexer);
  }

  // if true, this is a float
  if (lexer_peek_char(lexer) == '.') {
    lexer_read_char(lexer);
  } else {
    // regular int
    // total chars in this string
    *token_type = TOKEN_INTEGER;
    size_t total_chars = lexer->cur_pos - pos;
    char *str = strndup((lexer->input.data + pos), total_chars);
    if (sscanf(str, "%d", i32) == 0) {
      // failed to convert to string
      free(str);
      *token_type = TOKEN_INVALID;
      return false;
    }
    free(str);
    return true;
  }
  while (!LEXER_IS_END && isdigit(lexer->ch)) {
    lexer_read_char(lexer);
  }

  *token_type = TOKEN_FLOAT;
  size_t total_chars = lexer->cur_pos - pos;
  char *str = strndup((lexer->input.data + pos), total_chars);
  if (sscanf(str, "%f", f32) == 0) {
    // failed to convert to string
    *token_type = TOKEN_INVALID;
    free(str);
    return false;
  }
  free(str);
  return true;
}

StringView lexer_read_ident(Lexer *lexer) {
  size_t pos = lexer->cur_pos;
  while (!LEXER_IS_END && isalpha(lexer->ch)) {
    lexer_read_char(lexer);
  }
  return string_view_from_string_view(lexer->input, pos, lexer->cur_pos + 1);
}
