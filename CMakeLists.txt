cmake_minimum_required(VERSION 3.24)

# Place targets in bin/ or lib/ for all build configurations
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 17)

project(mil-sh C)

add_executable(mil-sh src/main.c)

target_compile_options(
  mil-sh
  PRIVATE -Wall
          -Wextra
          -pedantic
          -Wno-unused-parameter
          -Wstrict-prototypes
          -Wshadow
          -Wconversion
          -Wvla
          -Wdouble-promotion
          -Wmissing-noreturn
          -Wmissing-format-attribute
          -Wmissing-prototypes
          -fsigned-char)

