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
    Commands commands = parser_parse(&parser);
    if (commands.count == 0) {
      continue;
    }

    for (size_t i = 0; i < commands.count; ++i) {
      Command command = commands.data[i];
      if (command.type == EXIT_COMMAND_TYPE) {
        return;
      } else if (commands.data[i].type == ECHO_COMMAND_TYPE) {
        Arguments args = command.cmd.echo.args;
        assert(args.count > 0 && "echo command had 0 args");
        for (size_t j = 0; j < args.count; ++j) {
          Argument arg = args.data[j];
          switch (arg.type) {
          case ARGUMENT_IDENTIFIER:
            fprintf(stdout, "%.*s\n", (int)arg.value.identifier.count,
                    arg.value.identifier.data);
            break;
          case ARGUMENT_LITERAL_STRING:
            fprintf(stdout, "'%.*s'\n", (int)arg.value.string.count,
                    arg.value.string.data);
            break;
          case ARGUMENT_LITERAL_FLOAT:
            fprintf(stdout, "%f\n", (double)arg.value.f32);
            break;
          case ARGUMENT_LITERAL_INTEGER:
            fprintf(stdout, "%d\n", arg.value.i32);
            break;
          default:
            fprintf(stderr, "unhandled arg print");
            break;
          };
        }
        DA_FREE(&command.cmd.echo.args);
      }
    }
    DA_FREE(&commands);
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
