#include <core/error.h>
#include <module.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <value.h>

Value ffi_slice(size_t argc, Module *mod, Value *args) {
  ASSERT_FMT(argc == 3, "Expected 3 arguments, but got %zu", argc);

  char *str = args[0].string_value;
  int start = args[1].int_value;
  int end = args[2].int_value;

  int len = strlen(str);

  if (start < 0) start = len + start;
  if (end < 0) end = len + end;

  if (start < 0) start = 0;
  if (end < 0) end = 0;

  if (start > len) start = len;
  if (end > len) end = len;

  if (start > end) start = end;

  char *slice = malloc((end - start + 1) * sizeof(char));
  strncpy(slice, str + start, end - start);
  slice[end - start] = '\0';

  return MAKE_STRING(slice);
}