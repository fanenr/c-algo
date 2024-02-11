#ifndef LIST_H
#define LIST_H

#include <stdalign.h>
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

extern list_n *list_remove (list *restrict lis, list_n *restrict pos)
    __attribute__ ((nonnull (1, 2)));

extern list_n *list_push_back (list *restrict lis, void *restrict data,
                               size_t ele) __attribute__ ((nonnull (1, 2)));

extern list_n *list_push_front (list *restrict lis, void *restrict data,
                                size_t ele) __attribute__ ((nonnull (1, 2)));

extern list_n *list_insert (list *restrict lis, list_n *restrict pos,
                            void *restrict data, size_t ele)
    __attribute__ ((nonnull (1, 2, 3)));

#define LIST_DEF_TYPE(TYPE, PRE)                                              \
  typedef struct PRE##_list_n PRE##_list_n;                                   \
  struct PRE##_list_n                                                         \
  {                                                                           \
    PRE##_list_n *prev;                                                       \
    PRE##_list_n *next;                                                       \
    alignas (max_align_t) TYPE data[];                                        \
  };

#define LIST_DEF_AT(TYPE, PRE)                                                \
  __attribute__ ((nonnull (1))) static inline PRE##_list_n *PRE##_list_at (   \
      list *lis, size_t pos)                                                  \
  {                                                                           \
    return (PRE##_list_n *)list_at (lis, pos);                                \
  }

#define LIST_DEF_REMOVE(TYPE, PRE)                                            \
  __attribute__ ((nonnull (1, 2))) static inline PRE##_list_n                 \
      *PRE##_list_remove (list *restrict lis, PRE##_list_n *restrict pos)     \
  {                                                                           \
    return (PRE##_list_n *)list_remove (lis, (list_n *)pos);                  \
  }

#define LIST_DEF_PUSH_BACK(TYPE, PRE)                                         \
  __attribute__ ((nonnull (                                                   \
      1))) static inline PRE##_list_n *PRE##_list_push_back (list *lis,       \
                                                             TYPE data)       \
  {                                                                           \
    return (PRE##_list_n *)list_push_back (lis, &data, sizeof (TYPE));        \
  }

#define LIST_DEF_PUSH_FRONT(TYPE, PRE)                                        \
  __attribute__ ((nonnull (                                                   \
      1))) static inline PRE##_list_n *PRE##_list_push_front (list *lis,      \
                                                              TYPE data)      \
  {                                                                           \
    return (PRE##_list_n *)list_push_front (lis, &data, sizeof (TYPE));       \
  }

#define LIST_DEF_INSERT(TYPE, PRE)                                            \
  __attribute__ ((nonnull (1, 2))) static inline PRE##_list_n                 \
      *PRE##_list_insert (list *restrict lis, PRE##_list_n *restrict pos,     \
                          TYPE data)                                          \
  {                                                                           \
    return (PRE##_list_n *)list_insert (lis, (list_n *)pos, &data,            \
                                        sizeof (TYPE));                       \
  }

#define LIST_DEF_ALL(TYPE, PRE)                                               \
  LIST_DEF_TYPE (TYPE, PRE);                                                  \
  LIST_DEF_AT (TYPE, PRE);                                                    \
  LIST_DEF_REMOVE (TYPE, PRE);                                                \
  LIST_DEF_PUSH_BACK (TYPE, PRE);                                             \
  LIST_DEF_PUSH_FRONT (TYPE, PRE);                                            \
  LIST_DEF_INSERT (TYPE, PRE)

#endif
