#include <core/error.h>
#include <module.h>
#include <stdio.h>
#include <stdlib.h>
#include <value.h>
#include <dirent.h>

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

Value MAKE_NONE() {
  Value v;
  v.type = VALUE_LIST;
  v.list_value.length = 3;
  v.list_value.values = malloc(sizeof(Value) * 3);
  v.list_value.values[0] = MAKE_SPECIAL();
  v.list_value.values[1] = MAKE_STRING("Option");
  v.list_value.values[2] = MAKE_STRING("None");
  return v;
}

Value MAKE_SOME(Value value) {
  Value v;
  v.type = VALUE_LIST;
  v.list_value.length = 4;
  v.list_value.values = malloc(sizeof(Value) * 4);
  v.list_value.values[0] = MAKE_SPECIAL();
  v.list_value.values[1] = MAKE_STRING("Option");
  v.list_value.values[2] = MAKE_STRING("Some");
  v.list_value.values[3] = value;
  return v;
}

Value execute(size_t argc, Module *mod, Value *args) {
  ASSERT_FMT(argc == 1, "Expected 1 argument, but got %zu", argc);

  char *command = args[0].string_value;
  int ret = system(command);
  return MAKE_INTEGER(ret);
}

Value ppm_readfile(size_t argc, Module *mod, Value *args) {
  ASSERT_FMT(argc == 1, "Expected 1 argument, but got %zu", argc);

  char *filename = args[0].string_value;
  FILE *file = fopen(filename, "r");
  if (file == NULL) MAKE_NONE();

  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *buffer = malloc(size + 1);
  fread(buffer, 1, size, file);
  buffer[size] = '\0';

  fclose(file);
  return MAKE_SOME(MAKE_STRING(buffer));
}

Value ppm_writefile(size_t argc, Module *mod, Value *args) {
  ASSERT_FMT(argc == 2, "Expected 2 arguments, but got %zu", argc);

  char *filename = args[0].string_value;
  char *contents = args[1].string_value;

  FILE *file = fopen(filename, "w");
  if (file == NULL) return MAKE_INTEGER(0);

  fwrite(contents, 1, strlen(contents), file);
  fclose(file);
  return MAKE_INTEGER(1);
}

Value ppm_mkdir(size_t argc, Module *mod, Value *args) {
  ASSERT_FMT(argc == 1, "Expected 1 argument, but got %zu", argc);

  char *dirname = args[0].string_value;
  int ret = create_directory(dirname);
  return MAKE_INTEGER(ret);
}

Value get_cwd(size_t argc, Module *mod, Value *args) {
  char *cwd = malloc(FILENAME_MAX * sizeof(char));
  GetCurrentDir(cwd, FILENAME_MAX);
  if (cwd == NULL) return MAKE_NONE();
  return MAKE_SOME(MAKE_STRING(cwd));
}

Value get_env(size_t argc, Module *mod, Value *args) {
  ASSERT_FMT(argc == 1, "Expected 1 argument, but got %zu", argc);

  char *name = args[0].string_value;
  char *value = getenv(name);
  if (value == NULL) return MAKE_NONE();
  return MAKE_SOME(MAKE_STRING(value));
}

Value ppm_chmod(size_t argc, Module *mod, Value *args) {
  ASSERT_FMT(argc == 2, "Expected 2 arguments, but got %zu", argc);

  char *filename = args[0].string_value;
  mode_t mode = args[1].int_value;
  printf("chmod(%s, %d)\n", filename, mode);
  int ret = chmod(filename, mode);
  printf("%d\n", ret);
  return MAKE_INTEGER(ret);
}

Value which(size_t argc, Module *mod, Value *args) {
  ASSERT_FMT(argc == 1, "Expected 1 argument, but got %zu", argc);

  char *command = args[0].string_value;
  char *path = getenv("PATH");
  char *token = strtok(path, ":");

  while (token != NULL) {
    char *fullpath = malloc(strlen(token) + strlen(command) + 2);
    sprintf(fullpath, "%s/%s", token, command);

    if (access(fullpath, F_OK) != -1) {
      free(fullpath);
      return MAKE_SOME(MAKE_STRING(token));
    }

    free(fullpath);
    token = strtok(NULL, ":");
  }

  return MAKE_NONE();
}

Value does_directory_exist(size_t argc, Module *mod, Value *args) {
  ASSERT_FMT(argc == 1, "Expected 1 argument, but got %zu", argc);

  char *dirname = args[0].string_value;
  DIR *dir = opendir(dirname);
  if (dir == NULL) return MAKE_INTEGER(0);
  closedir(dir);
  return MAKE_INTEGER(1);
}