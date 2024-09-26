#include "parser.h"
#include "utils.h"
#include <stdbool.h>
#include <stdlib.h>

#define PARSER_PEEK_TOKEN_IS(parser, type)                                     \
  (parser->peek_token.token_type == (type))

void parser_next_token(Parser *parser) {
  Token next_token = {.token_type = TOKEN_EOF};
  while (1) {
    Token token = lexer_next_token(&parser->lexer);
    if (token.token_type == TOKEN_INVALID) {
      fprintf(stderr, "invalid token\n");
    } else {
      next_token = token;
      break;
    }
  }
  parser->peek_token = next_token;
}

Parser parser_new(char *buf, size_t len) {
  Lexer lexer = lexer_new((StringView){.data = buf, .count = len});
  Parser parser = {.lexer = lexer};
  parser_next_token(&parser);
  return parser;
}

Commands parser_parse(Parser *parser) {
  Commands commands = {0};
  commands.data = malloc(sizeof(Command));
  if (commands.data == NULL) {
    fprintf(stderr, "ran out of memory!!");
    return commands;
  }
  while (PARSER_PEEK_TOKEN_IS(parser, TOKEN_EOF)) {
    Command command = {0};
    if (PARSER_PEEK_TOKEN_IS(parser, TOKEN_EXIT)) {
      command.type = EXIT_COMMAND_TYPE;
    } else if (PARSER_PEEK_TOKEN_IS(parser, TOKEN_ECHO)) {
      command.type = ECHO_COMMAND_TYPE;
    }
    lexer_print_token(&parser->peek_token);
    DA_APPEND(&commands, command);
  }
  return commands;
}
// bool parse_echo_command(Parser* parser, EchoCommand* echo_command){
//
// }

Token parser_peek_token(Parser *parser) {}
