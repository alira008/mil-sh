#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    read_line(buf, SHELL_MAX_BUFF_LEN);
  } while (1);
}

size_t read_line(char *buf, size_t max_len) {
  int position = 0;
  while (1) {
    int c = getchar();

    if (c == EOF || c == '\n') {
      buf[position] = '\n';
      return position;
    } else {
      buf[position] = c;
    }

    ++position;
    if (position > max_len) {
      fprintf(stderr, "mil-sh: line was too long for the shell");
      exit(1);
    }
  }
}
