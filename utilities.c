#include ".h"

_Noreturn void die(const char *string, char x) {
  fprintf(stderr, "%s\n", string);
  exit(x);
}

