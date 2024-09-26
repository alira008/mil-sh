#include "lexer.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

#define SHELL_MAX_BUFF_LEN 1024

void run_shell(void);
size_t read_line(char *, size_t);

int main(void) {
  run_shell();
  return 0;
}

void run_shell(void) {
  char buf[SHELL_MAX_BUFF_LEN] = {0};
  do {
    size_t len = read_line(buf, SHELL_MAX_BUFF_LEN);
    StringView buf_sv = {.data = buf, .count = len};
    Lexer lexer = lexer_new(buf_sv);
    Tokens tokens = {0};
    tokens.data = malloc(sizeof(Token));
    Token token = lexer_next_token(&lexer);
    while (token.token_type != TOKEN_EOF) {
      if (token.token_type == TOKEN_EXIT) {
        printf("exit command\n");
        goto exit_shell;
      } else if (token.token_type == TOKEN_ECHO) {
        printf("echo command\n");
      } else if (token.token_type == TOKEN_IDENT) {
        printf("ident: %.*s\n", (int)token.literal.string.count,
               token.literal.string.data);
      } else if (token.token_type == TOKEN_FLAG_SINGLE_DASH) {
        printf("flag: -%.*s\n", (int)token.literal.string.count,
               token.literal.string.data);
      } else if (token.token_type == TOKEN_FLAG_DOUBLE_DASH) {
        printf("flag: --%.*s\n", (int)token.literal.string.count,
               token.literal.string.data);
      } else if (token.token_type == TOKEN_INVALID) {
        printf("invalid token\n");
      }
      DA_APPEND(&tokens, token);
      token = lexer_next_token(&lexer);
    }
    DA_FREE(&tokens);
  } while (1);
exit_shell:
  return;
}

size_t read_line(char *buf, size_t max_len) {
  size_t position = 0;
  while (1) {
    int c = getchar();

    if (c == EOF || c == '\n') {
      buf[position] = '\0';
      break;
    } else {
      buf[position++] = (char)c;
    }

    if (position > max_len - 2) {
      fprintf(stderr,
              "mil-sh: line was too long for the shell, max input allowed: %zu",
              max_len - 2);
      exit(1);
    }
  }

  return position;
}
