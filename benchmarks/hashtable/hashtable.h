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
  hashtable_node_t **slots;
};

struct hashtable_node_t
{
  size_t hash;
  hashtable_node_t *prev;
  hashtable_node_t *next;
};

#define HASHTABLE_INIT                                                        \
  (hashtable_t) {}

#define hashtable_head(ht, hash) ((ht)->slots[(hash) % (ht)->cap])

extern void hashtable_rehash (hashtable_node_t **slots, size_t cap,
                              hashtable_t *ht) attr_nonnull (1, 3);

extern void hashtable_link (hashtable_t *ht, hashtable_node_t **inpos,
                            hashtable_node_t *prev, hashtable_node_t *node)
    attr_nonnull (1, 2, 4);

extern void hashtable_erase (hashtable_t *ht, hashtable_node_t *node)
    attr_nonnull (1, 2);

#endif
