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

