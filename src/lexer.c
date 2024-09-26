#include "lexer.h"
#include <assert.h>
#include <ctype.h>
#include <string.h>

#define LEXER_IS_END (lexer->read_pos > lexer->input.count)
#define KEYWORDS_LEN 2
static String KEYWORDS_STRING[KEYWORDS_LEN] = {STRING_LIT("echo"),
                                               STRING_LIT("exit")};
static enum TokenType KEYWORDS_TOKENTYPE[KEYWORDS_LEN] = {TOKEN_ECHO,
                                                          TOKEN_EXIT};

StringView lexer_read_ident(Lexer *lexer);
void lexer_read_char(Lexer *lexer);
void lexer_skip_whitespace(Lexer *lexer);
char lexer_peek_char(Lexer *lexer);
StringView lexer_lex_string(Lexer *lexer);
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
  } else if (lexer->ch == '\0') {
    token.token_type = TOKEN_EOF;
  } else {
    token.token_type = TOKEN_INVALID;
  }

  lexer_read_char(lexer);
  return token;
}

StringView lexer_read_ident(Lexer *lexer) {
  size_t pos = lexer->cur_pos;
  while (!LEXER_IS_END && isalpha(lexer->ch)) {
    lexer_read_char(lexer);
  }
  return string_view_from_string_view(lexer->input, pos, lexer->cur_pos + 1);
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

void lexer_print_token(const Token *const token) {
  if (token->token_type == TOKEN_EXIT) {
    printf("exit command\n");
  } else if (token->token_type == TOKEN_ECHO) {
    printf("echo command\n");
  } else if (token->token_type == TOKEN_IDENT) {
    printf("ident: %.*s\n", (int)token->literal.string.count,
           token->literal.string.data);
  } else if (token->token_type == TOKEN_FLAG_SINGLE_DASH) {
    printf("flag: -%.*s\n", (int)token->literal.string.count,
           token->literal.string.data);
  } else if (token->token_type == TOKEN_FLAG_DOUBLE_DASH) {
    printf("flag: --%.*s\n", (int)token->literal.string.count,
           token->literal.string.data);
  } else if (token->token_type == TOKEN_INVALID) {
    printf("invalid token\n");
  }
}
