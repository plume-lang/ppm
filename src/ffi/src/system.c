#include <core/error.h>
#include <module.h>
#include <stdio.h>
#include <stdlib.h>
#include <value.h>

Value execute(size_t argc, Module *mod, Value *args) {
  ASSERT_FMT(argc == 1, "Expected 1 argument, but got %d", argc);

  char *command = args[0].string_value;
  int ret = system(command);
  return MAKE_INTEGER(ret);
}