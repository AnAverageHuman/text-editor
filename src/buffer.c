#include ".h"

#define REALLOC_CHUNK 256

/* Simple buffer specification:
 * A buffer is a linked list structure, where each node represents a "line" of
 * text. Each node has a few attributes, including the contents (stored as a
 * contiguous chunk of memory), the current length, the allocated size, the
 * previous node, and the next node (i.e. the lines above and below).
 */

node *add_node(node *buf, node *prev, node *next, char *contents,
    unsigned long long length, unsigned long long allocd) {
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

node *add_char_to_node(node *buf, char character, unsigned long long pos) {
  if (buf->length == buf->allocated) {
    buf->contents = realloc(buf->contents,
        sizeof(buf->contents[0]) * (buf->allocated + REALLOC_CHUNK));
    buf->allocated += REALLOC_CHUNK;
  }

  memmove(buf->contents + pos + 1, buf->contents + pos, buf->length - pos + 1);
  buf->contents[pos] = character;
  buf->length++;
  return buf;
}

node *del_char_from_node(node *buf, unsigned long long pos) {
  memmove(buf->contents + pos - 1, buf->contents + pos, buf->length - pos + 1);
  buf->length--;  // don't reallocate as that's expensive
  return buf;
}

node *init_buffer(node *buf, unsigned long long rows) {
  buf = add_node(malloc(sizeof(*buf)), 0, 0, 0, 0, 0);
  unsigned long long i;

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

