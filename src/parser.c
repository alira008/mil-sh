#include "parser.h"
#include "utils.h"
#include <stdlib.h>

Parser parser_new(char *buf, size_t len) {
  Lexer lexer = lexer_new((StringView){.data = buf, .count = len});
  Tokens tokens = {0};
  tokens.data = malloc(sizeof(Token));
  return (Parser){.lexer = lexer, .tokens = tokens};
}

void parser_parse(Parser *parser) {
  Token token = lexer_next_token(&parser->lexer);
  while (token.token_type != TOKEN_EOF) {
    if (token.token_type == TOKEN_EXIT) {
    } else if (token.token_type == TOKEN_ECHO) {
    }
    lexer_print_token(&token);
    DA_APPEND(&parser->tokens, token);
    token = lexer_next_token(&parser->lexer);
  }
}

void parser_free_tokens(Parser *parser) { DA_FREE(&parser->tokens); }
