#include ".h"

// If we can handle it, do it here. Else return for further processing.
int errno_handler(int en) {
  errno = 0;
  switch (en) {
    case 0:
      return 0;
    case EACCES:
      return EACCES;
    default:
      fprintf(stderr, "Error %d: %s\n", en, strerror(en));
      return -1;
  }
}

_Noreturn void die(const char *string, char x) {
  fprintf(stderr, "%s\n", string);
  exit(x);
}

