#include "lexer.h"
#include <assert.h>
#include <ctype.h>

#define LEXER_IS_END (lexer->read_pos >= lexer->input.count)

void read_char(Lexer *lexer);
void skip_whitespace(Lexer *lexer);
char peek_char(Lexer *lexer);
StringView lexer_lex_string(Lexer *lexer);

Lexer lexer_new(StringView input) {
  Lexer lexer = {.input = input, .ch = '\0'};
  read_char(&lexer);
  return lexer;
}

Token lexer_next_token(Lexer *lexer) {
  char c = lexer->input.data[lexer->cur_pos];
  Token token = {0};
  if (isalpha(c)) {
    StringView string_view = lexer_lex_string(lexer);
  }

  return token;
}

void read_char(Lexer *lexer) {
  if (LEXER_IS_END) {
    lexer->ch = '\0';
  } else {
    lexer->ch = lexer->input.data[lexer->read_pos];
  }

  lexer->cur_pos = lexer->read_pos;
  ++lexer->read_pos;
}

void skip_whitespace(Lexer *lexer) {
  while (isspace(lexer->ch) && lexer->ch != '\0') {
    read_char(lexer);
  }
}

char peek_char(Lexer *lexer) {
  if (LEXER_IS_END)
    return '\0';
  return lexer->input.data[lexer->read_pos];
}
