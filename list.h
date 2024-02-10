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

#define LIST_DEFS(TYPE, PRE)                                                  \
  typedef struct PRE##_list_n PRE##_list_n;                                   \
                                                                              \
  struct PRE##_list_n                                                         \
  {                                                                           \
    PRE##_list_n *prev;                                                       \
    PRE##_list_n *next;                                                       \
    TYPE data[];                                                              \
  };                                                                          \
                                                                              \
  __attribute__ ((nonnull (1))) static inline PRE##_list_n *PRE##_list_at (   \
      list *lis, size_t pos)                                                  \
  {                                                                           \
    return (PRE##_list_n *)list_at (lis, pos);                                \
  }                                                                           \
                                                                              \
  __attribute__ ((nonnull (1, 2))) static inline PRE##_list_n                 \
      *PRE##_list_remove (list *lis, PRE##_list_n *pos)                       \
  {                                                                           \
    return (PRE##_list_n *)list_remove (lis, (list_n *)pos);                  \
  }                                                                           \
                                                                              \
  __attribute__ ((nonnull (                                                   \
      1))) static inline PRE##_list_n *PRE##_list_push_back (list *lis,       \
                                                             TYPE data)       \
  {                                                                           \
    return (PRE##_list_n *)list_push_back (lis, &data, sizeof (TYPE));        \
  }                                                                           \
                                                                              \
  __attribute__ ((nonnull (                                                   \
      1))) static inline PRE##_list_n *PRE##_list_push_front (list *lis,      \
                                                              TYPE data)      \
  {                                                                           \
    return (PRE##_list_n *)list_push_front (lis, &data, sizeof (TYPE));       \
  }                                                                           \
                                                                              \
  __attribute__ ((nonnull (1, 2))) static inline PRE##_list_n                 \
      *PRE##_list_insert (list *lis, PRE##_list_n *pos, TYPE data)            \
  {                                                                           \
    return (PRE##_list_n *)list_insert (lis, (list_n *)pos, &data,            \
                                        sizeof (TYPE));                       \
  }

#endif
