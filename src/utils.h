#pragma once

#include <stdio.h>

typedef struct {
  char *data;
  size_t count;
  size_t capacity;
} String;

typedef struct {
  String *data;
  size_t count;
  size_t capacity;
} Strings;

typedef struct {
  char *data;
  size_t count;
} StringView;

#define DA_INIT_CAP 256
#define DA_FREE(da) free((da)->data)
#define DA_APPEND(da, item)                                                    \
  do {                                                                         \
    if ((da)->count >= (da)->capacity - 1) {                                   \
      size_t new_cap = (da)->capacity == 0 ? DA_INIT_CAP : (da)->capacity * 2; \
      void *new_ptr = realloc((da), (da)->capacity * sizeof(*(da)->data));     \
      assert(new_ptr != NULL && "Ran out of ram!!");                           \
      (da)->capacity = new_cap;                                                \
      (da)->data = new_ptr;                                                    \
    }                                                                          \
    (da)->data[(da)->count++] = (item);                                        \
  } while (0)

#define STRING_LIT(str)                                                        \
  ((String){.data = str, .count = sizeof(str), .capacity = sizeof(str)})
#define STRING_INIT(str)                                                       \
  ((String){.data = str, .count = strlen(str), .capacity = strlen(str)})
#define STRING_EMPTY_INIT                                                      \
  ((String){.data = NULL, .count = 0, .capacity = DA_INIT_CAP})

static StringView string_view_from_string(String string, size_t start, size_t end);

