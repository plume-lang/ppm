#include <core/error.h>
#include <ctype.h>
#include <module.h>
#include <stdio.h>
#include <stdlib.h>
#include <value.h>

Value is_alphabetic(size_t argc, Module *mod, Value *args) {
  ASSERT_FMT(argc == 1, "Expected 1 argument, but got %zu", argc);

  char *str = args[0].string_value;
  size_t l = strlen(str);
  ASSERT_FMT(l == 1, "Expected 1 character, but got %zu", l);

  char c = str[0];

  return MAKE_INTEGER(isalpha(c));
}

Value is_digit(size_t argc, Module *mod, Value *args) {
  ASSERT_FMT(argc == 1, "Expected 1 argument, but got %zu", argc);

  char *str = args[0].string_value;
  size_t l = strlen(str);
  ASSERT_FMT(l == 1, "Expected 1 character, but got %zu", l);

  char c = str[0];

  return MAKE_INTEGER(isdigit(c));
}

Value is_alphanumeric(size_t argc, Module *mod, Value *args) {
  ASSERT_FMT(argc == 1, "Expected 1 argument, but got %zu", argc);

  char *str = args[0].string_value;
  size_t l = strlen(str);
  ASSERT_FMT(l == 1, "Expected 1 character, but got %zu", l);

  char c = str[0];

  return MAKE_INTEGER(isalnum(c));
}

Value is_whitespace(size_t argc, Module *mod, Value *args) {
  ASSERT_FMT(argc == 1, "Expected 1 argument, but got %zu", argc);

  char *str = args[0].string_value;
  size_t l = strlen(str);
  ASSERT_FMT(l == 1, "Expected 1 character, but got %zu", l);

  char c = str[0];

  return MAKE_INTEGER(isspace(c));
}