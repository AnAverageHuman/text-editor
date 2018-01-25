#include ".h"

/* Simple buffer specification:
 * A buffer is a linked list structure, where each node represents a "line" of
 * text. Each node has a few attributes, including the contents (stored as a
 * contiguous chunk of memory), the current length, the allocated size, the
 * previous node, and the next node (i.e. the lines above and below).
 */

node *add_node(node *buf, node *prev, node *next, char *contents,
    long long length, long long allocd) {
  if (prev) {
    prev->next = buf;
    buf->prev = prev;
  }
  if (next) {
    buf->next = next;
    next->prev = buf;
  }
  buf->contents = contents;
  buf->length = length ? length : (contents ? strlen(contents) : 0);
  buf->allocated = allocd ? allocd : buf->length;
  return buf;
}

node *init_buffer(node *buf, unsigned long long rows) {
  buf = add_node(malloc(sizeof(*buf)), 0, 0, 0, 0, 0);
  long long i;

  node *prev = buf;
  for (i = 0; i < rows - 1; i++) {
    prev = add_node(malloc(sizeof(*buf)), prev, 0, 0, 0, 0);
  }

  return buf;
}

node *read_into_buffer(node *buf, char *filename) {
  FILE *fp = fopen(filename, "r");
  node *prev = 0;
  node *current = buf;

  char *line;
  size_t allocd = 0;
  ssize_t length = 0;

  while ((length = getline(&line, &allocd, fp)) != -1) {
    if (current) {
      current->contents = line;
      current->allocated = allocd;
      current->length = length;
      prev = current;
      current = current->next;
    } else {
      prev = add_node(malloc(sizeof(*buf)), prev, 0, line, length, allocd);
    }

    allocd = 0;
  }

  errno_handler(errno);
  fclose(fp);
  return buf;
}

