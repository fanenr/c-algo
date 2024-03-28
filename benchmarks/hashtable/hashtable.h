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

#define HASHTABLE_INIT                                                        \
  (hashtable_t) {}

#define hashtable_head(ht, hash) ((ht)->data[(hash) % (ht)->cap])

extern void hashtable_move (hashtable_node_t **data, size_t cap,
                            hashtable_t *old) attr_nonnull (1, 3);

extern void hashtable_insert (hashtable_t *ht, size_t hash,
                              hashtable_node_t *node) attr_nonnull (1, 3);

extern void hashtable_erase (hashtable_t *ht, hashtable_node_t *node)
    attr_nonnull (1, 2);

#endif
