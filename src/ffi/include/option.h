#ifndef OPTION_H
#define OPTION_H

#include <value.h>
#include <stdlib.h>

static inline Value MAKE_NONE() {
  Value *ls = malloc(sizeof(Value) * 3);
  ls[0] = MAKE_SPECIAL();
  ls[1] = MAKE_STRING("Option", 6);
  ls[2] = MAKE_STRING("None", 4);

  return MAKE_LIST(ls, 3);
}

static inline Value MAKE_SOME(Value value) {
  Value *ls = malloc(sizeof(Value) * 4);
  ls[0] = MAKE_SPECIAL();
  ls[1] = MAKE_STRING("Option", 6);
  ls[2] = MAKE_STRING("Some", 4);
  ls[3] = value;
  return MAKE_LIST(ls, 4);
}
#endif // OPTION_H