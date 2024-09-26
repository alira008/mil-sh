#include "parser.h"
#include "utils.h"
#include <stdbool.h>
#include <stdlib.h>

#define PARSER_PEEK_TOKEN_IS(parser, type)                                     \
  (parser->peek_token.token_type == type)

void parser_next_token(Parser *parser);
bool parse_echo_command(Parser *parser, EchoCommand *echo_command);

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
  while (!PARSER_PEEK_TOKEN_IS(parser, TOKEN_EOF)) {
    Command command = {0};
    if (PARSER_PEEK_TOKEN_IS(parser, TOKEN_EXIT)) {
      command.type = EXIT_COMMAND_TYPE;
      parser_next_token(parser);
    } else if (PARSER_PEEK_TOKEN_IS(parser, TOKEN_ECHO)) {
      command.type = ECHO_COMMAND_TYPE;

      EchoCommand echo_command = {0};
      if (!parse_echo_command(parser, &echo_command)) {
        fprintf(stderr, "empty echo command\n");
        parser_next_token(parser);
        break;
      };
      command.cmd.echo = echo_command;
    } else {
      lexer_print_token(stderr, &parser->peek_token);
      fprintf(stderr, " is an unknown command to mil-sh\n");
      parser_next_token(parser);
      break;
    }
    DA_APPEND(&commands, command);
  }
  return commands;
}

bool parse_echo_command(Parser *parser, EchoCommand *echo_command) {
  // skip echo keyword
  parser_next_token(parser);
  echo_command->args.data = malloc(sizeof(Argument));

  while (1) {
    Argument arg = {0};
    if (PARSER_PEEK_TOKEN_IS(parser, TOKEN_IDENT)) {
      arg.type = ARGUMENT_IDENTIFIER;
      arg.value.identifier = parser->peek_token.literal.string;
    } else if (PARSER_PEEK_TOKEN_IS(parser, TOKEN_FLOAT)) {
      arg.type = ARGUMENT_LITERAL_FLOAT;
      arg.value.f32 = parser->peek_token.literal.f32;
    } else if (PARSER_PEEK_TOKEN_IS(parser, TOKEN_INTEGER)) {
      arg.type = ARGUMENT_LITERAL_INTEGER;
      arg.value.i32 = parser->peek_token.literal.i32;
    } else {
      break;
    }
    DA_APPEND(&echo_command->args, arg);

    parser_next_token(parser);
  }
  if (echo_command->args.count == 0) {
    DA_FREE(&echo_command->args);
    return false;
  }

  return true;
}

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
