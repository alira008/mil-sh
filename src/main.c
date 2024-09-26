#include "lexer.h"
#include "parser.h"
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
    fprintf(stdout, "mil-sh > ");
    size_t len = read_line(buf, SHELL_MAX_BUFF_LEN);
    Parser parser = parser_new(buf, len);
    parser_parse(&parser);
    parser_free_tokens(&parser);
    return;
  } while (1);
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
