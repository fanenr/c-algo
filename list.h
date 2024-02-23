#ifndef LIST_H
#define LIST_H

#include <stddef.h>

typedef struct list_i
{
  size_t d_size;
  size_t n_size;
  size_t d_offs;
} list_i;

typedef struct list_n list_n;

struct list_n
{
  list_n *prev;
  list_n *next;
};

typedef struct list
{
  size_t len;
  list_n *head;
  list_n *tail;
} list;

extern void list_init (list *lis) __attribute__ ((nonnull (1)));

extern void list_free (list *lis) __attribute__ ((nonnull (1)));

extern void list_remove (list *lis, list_n *pos)
    __attribute__ ((nonnull (1, 2)));

extern list_n *list_at (const list *lis, size_t pos)
    __attribute__ ((nonnull (1)));

extern list_n *list_push_back (list *lis, void *data, const list_i *info)
    __attribute__ ((nonnull (1, 2, 3)));

extern list_n *list_push_front (list *lis, void *data, const list_i *info)
    __attribute__ ((nonnull (1, 2, 3)));

extern list_n *list_insert (list *lis, list_n *pos, void *data,
                            const list_i *info)
    __attribute__ ((nonnull (1, 2, 3, 4)));

#define LIST_DEF_AT(TYPE, PRE)                                                \
  static inline PRE##_list_n *PRE##_list_at (const list *lis, size_t pos)     \
  {                                                                           \
    return (PRE##_list_n *)list_at (lis, pos);                                \
  }

#define LIST_DEF_REMOVE(TYPE, PRE)                                            \
  static inline void PRE##_list_remove (list *lis, PRE##_list_n *pos)         \
  {                                                                           \
    list_remove (lis, (list_n *)pos);                                         \
  }

#define LIST_DEF_PUSH_BACK(TYPE, PRE)                                         \
  static inline PRE##_list_n *PRE##_list_push_back (list *lis, TYPE data)     \
  {                                                                           \
    return (PRE##_list_n *)list_push_back (lis, &data, &PRE##_list_info);     \
  }

#define LIST_DEF_PUSH_FRONT(TYPE, PRE)                                        \
  static inline PRE##_list_n *PRE##_list_push_front (list *lis, TYPE data)    \
  {                                                                           \
    return (PRE##_list_n *)list_push_front (lis, &data, &PRE##_list_info);    \
  }

#define LIST_DEF_INSERT(TYPE, PRE)                                            \
  static inline PRE##_list_n *PRE##_list_insert (                             \
      list *lis, PRE##_list_n *pos, TYPE data)                                \
  {                                                                           \
    return (PRE##_list_n *)list_insert (lis, (list_n *)pos, &data,            \
                                        &PRE##_list_info);                    \
  }

#define LIST_DEF_INFO(TYPE, PRE)                                              \
  typedef struct PRE##_list_n PRE##_list_n;                                   \
                                                                              \
  struct PRE##_list_n                                                         \
  {                                                                           \
    PRE##_list_n *prev;                                                       \
    PRE##_list_n *next;                                                       \
    TYPE data;                                                                \
  };                                                                          \
                                                                              \
  static const list_i PRE##_list_info                                         \
      = { .d_size = sizeof (TYPE),                                            \
          .n_size = sizeof (PRE##_list_n),                                    \
          .d_offs = offsetof (PRE##_list_n, data) };                          \
                                                                              \
  static void PRE##_list_remove (list *lis, PRE##_list_n *pos)                \
      __attribute__ ((nonnull (1, 2)));                                       \
                                                                              \
  static PRE##_list_n *PRE##_list_at (const list *lis, size_t pos)            \
      __attribute__ ((nonnull (1)));                                          \
                                                                              \
  static inline PRE##_list_n *PRE##_list_push_back (list *lis, TYPE data)     \
      __attribute__ ((nonnull (1)));                                          \
                                                                              \
  static inline PRE##_list_n *PRE##_list_push_front (list *lis, TYPE data)    \
      __attribute__ ((nonnull (1)));                                          \
                                                                              \
  static inline PRE##_list_n *PRE##_list_insert (                             \
      list *lis, PRE##_list_n *pos, TYPE data)                                \
      __attribute__ ((nonnull (1, 2)));

#define LIST_DEF_ALL(TYPE, PRE)                                               \
  LIST_DEF_INFO (TYPE, PRE);                                                  \
  LIST_DEF_AT (TYPE, PRE);                                                    \
  LIST_DEF_REMOVE (TYPE, PRE);                                                \
  LIST_DEF_PUSH_BACK (TYPE, PRE);                                             \
  LIST_DEF_PUSH_FRONT (TYPE, PRE);                                            \
  LIST_DEF_INSERT (TYPE, PRE)

#endif
