#include <core/error.h>
#include <module.h>
#include <stdio.h>
#include <stdlib.h>
#include <value.h>
#include <dirent.h>
#include <option.h>

#if defined(_WIN32)
  #include <direct.h>
  #define GetCurrentDir _getcwd
#else
  #include <sys/stat.h>
  #include <unistd.h>
  #define GetCurrentDir getcwd
#endif

int create_directory(char* dirname) {
   #if defined(_WIN32)
    return _mkdir(dirname);
  #else 
    return mkdir(dirname, 0777); 
  #endif
}

Value execute(size_t argc, Module *mod, Value *args) {
  ASSERT_FMT(argc == 1, "Expected 1 argument, but got %zu", argc);

  char *command = GET_STRING(args[0]);
  int ret = system(command);
  return MAKE_INTEGER(ret);
}


// Read a file and returns None if the file does not exist or error occurs, otherwise returns Some(contents)
Value ppm_readfile(size_t argc, Module *mod, Value *args) {
  ASSERT_FMT(argc == 1, "Expected 1 argument, but got %zu", argc);

  char *filename = GET_STRING(args[0]);
  FILE *file = fopen(filename, "r");
  if (file == NULL) return MAKE_NONE();

  fseek(file, 0, SEEK_END);
  long length = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *contents = malloc(length + 1);
  fread(contents, 1, length, file);
  contents[length] = '\0';

  fclose(file);
  return MAKE_SOME(MAKE_STRING(contents, length));
}

Value ppm_writefile(size_t argc, Module *mod, Value *args) {
  ASSERT_FMT(argc == 2, "Expected 2 arguments, but got %zu", argc);

  char *filename = GET_STRING(args[0]);
  char *contents = GET_STRING(args[1]);

  FILE *file = fopen(filename, "w");
  if (file == NULL) return MAKE_INTEGER(0);

  fwrite(contents, 1, strlen(contents), file);
  fclose(file);
  return MAKE_INTEGER(1);
}

Value ppm_mkdir(size_t argc, Module *mod, Value *args) {
  ASSERT_FMT(argc == 1, "Expected 1 argument, but got %zu", argc);

  char *dirname = GET_STRING(args[0]);
  int ret = create_directory(dirname);
  return MAKE_INTEGER(ret);
}

Value get_cwd(size_t argc, Module *mod, Value *args) {
  char *cwd = malloc(FILENAME_MAX * sizeof(char));
  GetCurrentDir(cwd, FILENAME_MAX);
  if (cwd == NULL) return MAKE_NONE();
  return MAKE_SOME(MAKE_STRING(cwd, strlen(cwd)));
}

Value get_env(size_t argc, Module *mod, Value *args) {
  ASSERT_FMT(argc == 1, "Expected 1 argument, but got %zu", argc);

  char *name = GET_STRING(args[0]);
  char *value = getenv(name);
  if (value == NULL) return MAKE_NONE();
  return MAKE_SOME(MAKE_STRING(value, strlen(value)));
}

Value ppm_chmod(size_t argc, Module *mod, Value *args) {
  ASSERT_FMT(argc == 2, "Expected 2 arguments, but got %zu", argc);

  char *filename = GET_STRING(args[0]);
  mode_t mode = GET_INT(args[1]);
  printf("chmod(%s, %d)\n", filename, mode);
  int ret = chmod(filename, mode);
  printf("%d\n", ret);
  return MAKE_INTEGER(ret);
}

Value which(size_t argc, Module *mod, Value *args) {
  ASSERT_FMT(argc == 1, "Expected 1 argument, but got %zu", argc);

  char *command = GET_STRING(args[0]);
  char *path = getenv("PATH");
  char *token = strtok(path, ":");

  while (token != NULL) {
    char *fullpath = malloc(strlen(token) + strlen(command) + 2);
    sprintf(fullpath, "%s/%s", token, command);

    if (access(fullpath, F_OK) != -1) {
      free(fullpath);
      return MAKE_SOME(MAKE_STRING(token, strlen(token)));
    }

    free(fullpath);
    token = strtok(NULL, ":");
  }

  return MAKE_NONE();
}

Value does_directory_exist(size_t argc, Module *mod, Value *args) {
  ASSERT_FMT(argc == 1, "Expected 1 argument, but got %zu", argc);

  char *dirname = GET_STRING(args[0]);
  DIR *dir = opendir(dirname);
  if (dir == NULL) return MAKE_INTEGER(0);
  closedir(dir);
  return MAKE_INTEGER(1);
}

void print_helper(Value v) {
  switch (get_type(v)) {
    case TYPE_INTEGER:
      printf("%d", (int) GET_INT(v));
      break;
    case TYPE_FLOAT: 
      printf("%f", GET_FLOAT(v));
      break;
    case TYPE_STRING:
      printf("%s", GET_STRING(v));
      break;
    case TYPE_MUTABLE: {
      HeapValue* mutable = GET_PTR(v);
      printf("<mutable %p>", mutable);
      break;
    }
    case TYPE_FUNCTION:
      printf("<function>");
      break;
    case TYPE_FUNCENV:
      printf("<funcenv>");
      break;
    case TYPE_SPECIAL:
      printf("<special>");
      break;
    case TYPE_LIST: {
      printf("[");
      HeapValue* list = GET_PTR(v);
      for (size_t i = 0; i < list->length; i++) {
        print_helper(list->as_ptr[i]);
        if (i != list->length - 1) printf(", ");
      }
      printf("]");
      break;
    }
    case TYPE_UNKNOWN: default:
      printf("Unknown type: %d", get_type(v));
  }
}

Value debug(size_t argc, Module *mod, Value *args) {
  ASSERT_FMT(argc == 1, "Expected 1 argument, but got %zu", argc);

  print_helper(args[0]);
  printf("\n");

  return MAKE_INTEGER(0);
}