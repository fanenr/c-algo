#ifndef LIST_H
#define LIST_H

#include "util.h"
#include <stddef.h>

typedef struct list_t list_t;
typedef struct list_node_t list_node_t;

struct list_node_t
{
  list_node_t *prev;
  list_node_t *next;
};

struct list_t
{
  size_t size;
  list_node_t *head;
  list_node_t *tail;
};

#define LIST_INIT                                                             \
  (list_t) {}

extern list_node_t *list_at (const list_t *list, size_t index)
    attr_nonnull (1);

extern void list_push_front (list_t *list, list_node_t *node)
    attr_nonnull (1, 2);

extern void list_push_back (list_t *list, list_node_t *node)
    attr_nonnull (1, 2);

extern void list_insert_at (list_t *list, size_t index, list_node_t *node)
    attr_nonnull (1, 3);

extern void list_insert_front (list_t *list, list_node_t *pos,
                               list_node_t *node) attr_nonnull (1, 2, 3);

extern void list_insert_back (list_t *list, list_node_t *pos,
                              list_node_t *node) attr_nonnull (1, 2, 3);

extern void list_erase (list_t *list, list_node_t *node) attr_nonnull (1, 2);

extern void list_erase_at (list_t *list, size_t index) attr_nonnull (1);

extern void list_pop_front (list_t *list) attr_nonnull (1);

extern void list_pop_back (list_t *list) attr_nonnull (1);

#endif
