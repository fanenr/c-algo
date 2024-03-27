#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "util.h"
#include <stddef.h>

typedef struct hashtable_t hashtable_t;
typedef struct hashtable_node_t hashtable_node_t;

struct hashtable_t
{
  size_t cap;
  size_t size;
  hashtable_node_t **data;
};

struct hashtable_node_t
{
  size_t hash;
  hashtable_node_t *next;
  hashtable_node_t *prev;
};

#define HASHTABLE_INIT(CAP, DATA)                                             \
  (hashtable_t) { .cap = (CAP), .data = (DATA) }

#define hashtable_head(table, hash) ((table)->data[(hash) % (table)->cap])

extern hashtable_t *hashtable_move (hashtable_t *table, hashtable_t *origin)
    attr_nonnull (1);

extern hashtable_node_t *hashtable_insert (hashtable_t *table, size_t hash,
                                           hashtable_node_t *node)
    attr_nonnull (1, 3);

extern void hashtable_erase (hashtable_t *table, hashtable_node_t *node)
    attr_nonnull (1, 2);

#endif
