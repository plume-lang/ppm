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

Value str_split(size_t argc, Module *mod, Value *args) {
  ASSERT_FMT(argc == 2, "Expected 2 arguments, but got %zu", argc);

  char *str = args[0].string_value;
  char *delim = args[1].string_value;

  char *token = strtok(str, delim);
  Value *values = malloc(sizeof(Value) * 100);
  size_t i = 0;

  while (token != NULL) {
    values[i] = MAKE_STRING(token);
    token = strtok(NULL, delim);
    i++;
  }

  Value v;
  v.type = VALUE_LIST;
  v.list_value.length = i;
  v.list_value.values = values;

  return v;
}

Value ffi_to_int(size_t argc, Module *mod, Value *args) {
  ASSERT_FMT(argc == 1, "Expected 1 argument, but got %zu", argc);

  char *str = args[0].string_value;
  return MAKE_INTEGER(atoi(str));
}