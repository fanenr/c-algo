#ifndef LIST_H
#define LIST_H

#include <stddef.h>

typedef struct list_n list_n;

struct list_n
{
  list_n *prev;
  list_n *next;
  max_align_t data[];
};

typedef struct list
{
  size_t len;
  list_n *head;
  list_n *tail;
} list;

extern void list_init (list *lis) __attribute__ ((nonnull (1)));

extern void list_free (list *lis) __attribute__ ((nonnull (1)));

extern list_n *list_at (list *lis, size_t pos) __attribute__ ((nonnull (1)));

extern list_n *list_remove (list *lis, list_n *pos)
    __attribute__ ((nonnull (1, 2)));

extern list_n *list_push_back (list *lis, void *data, size_t ele)
    __attribute__ ((nonnull (1, 2)));

extern list_n *list_push_front (list *lis, void *data, size_t ele)
    __attribute__ ((nonnull (1, 2)));

extern list_n *list_insert (list *lis, list_n *pos, void *data, size_t ele)
    __attribute__ ((nonnull (1, 2, 3)));

#endif
