#pragma once

#include "utils.h"
enum CommandType { ECHO_COMMAND_TYPE, EXIT_COMMAND_TYPE };
enum ArgumentType {
  ARGUMENT_LITERAL_STRING,
  ARGUMENT_LITERAL_FLOAT,
  ARGUMENT_LITERAL_INTEGER
};
typedef struct {
  enum ArgumentType argument_type;
  union {
    StringView arg;
  } Type;
} Argument;

typedef struct {
  Argument *data;
  size_t count;
  size_t capacity;
} Arguments;

typedef struct {
  Arguments args;
} EchoCommand;

typedef struct {
  enum CommandType type;
  union {
    EchoCommand echo;
  } cmd;
} Command;

typedef struct {
  Command *data;
  size_t count;
  size_t capacity;
} Commands;
