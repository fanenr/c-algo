#ifndef LIST_H
#define LIST_H

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

#define list_node_init_v ((list_node_t){})

#define list_nonnull(...) __attribute__ ((nonnull (__VA_ARGS__)))

extern void list_init (list_t *list) list_nonnull (1);

extern list_node_t *list_at (const list_t *list, size_t index)
    list_nonnull (1);

extern list_node_t *list_push_back (list_t *list, list_node_t *node)
    list_nonnull (1, 2);

extern list_node_t *list_push_front (list_t *list, list_node_t *node)
    list_nonnull (1, 2);

extern list_node_t *list_insert (list_t *list, list_node_t *pos,
                                 list_node_t *node) list_nonnull (1, 2, 3);

extern void list_remove (list_t *list, list_node_t *node) list_nonnull (1, 2);

#undef list_nonnull

#endif
