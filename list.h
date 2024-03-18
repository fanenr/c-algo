#ifndef LIST_H
#define LIST_H

#include <stddef.h>

typedef struct list list;
typedef struct list_i list_i;
typedef struct list_n list_n;

struct list_i
{
  size_t d_size;
  size_t n_size;
  size_t d_offs;
};

struct list_n
{
  list_n *prev;
  list_n *next;
};

struct list
{
  size_t size;
  list_n *head;
  list_n *tail;
};

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

#define LIST_DEF_INIT(PRE, DTYPE)                                             \
  static inline void PRE##_list_init (PRE##_list *lis)                        \
  {                                                                           \
    return list_init ((list *)lis);                                           \
  }

#define LIST_DEF_FREE(PRE, DTYPE)                                             \
  static inline void PRE##_list_free (PRE##_list *lis)                        \
  {                                                                           \
    return list_free ((list *)lis);                                           \
  }

#define LIST_DEF_AT(PRE, DTYPE)                                               \
  static inline PRE##_list_n *PRE##_list_at (const PRE##_list *lis,           \
                                             size_t pos)                      \
  {                                                                           \
    return (PRE##_list_n *)list_at ((list *)lis, pos);                        \
  }

#define LIST_DEF_REMOVE(PRE, DTYPE)                                           \
  static inline void PRE##_list_remove (PRE##_list *lis, PRE##_list_n *pos)   \
  {                                                                           \
    list_remove ((list *)lis, (list_n *)pos);                                 \
  }

#define LIST_DEF_PUSH_BACK(PRE, DTYPE)                                        \
  static inline PRE##_list_n *PRE##_list_push_back (PRE##_list *lis,          \
                                                    DTYPE data)               \
  {                                                                           \
    return (PRE##_list_n *)list_push_back ((list *)lis, &data,                \
                                           &PRE##_list_info);                 \
  }

#define LIST_DEF_PUSH_FRONT(PRE, DTYPE)                                       \
  static inline PRE##_list_n *PRE##_list_push_front (PRE##_list *lis,         \
                                                     DTYPE data)              \
  {                                                                           \
    return (PRE##_list_n *)list_push_front ((list *)lis, &data,               \
                                            &PRE##_list_info);                \
  }

#define LIST_DEF_INSERT(PRE, DTYPE)                                           \
  static inline PRE##_list_n *PRE##_list_insert (                             \
      PRE##_list *lis, PRE##_list_n *pos, DTYPE data)                         \
  {                                                                           \
    return (PRE##_list_n *)list_insert ((list *)lis, (list_n *)pos, &data,    \
                                        &PRE##_list_info);                    \
  }

#define LIST_DEF_INFO(PRE, DTYPE)                                             \
  typedef struct PRE##_list PRE##_list;                                       \
  typedef struct PRE##_list_n PRE##_list_n;                                   \
                                                                              \
  struct PRE##_list                                                           \
  {                                                                           \
    size_t size;                                                              \
    PRE##_list_n *prev;                                                       \
    PRE##_list_n *next;                                                       \
  };                                                                          \
                                                                              \
  struct PRE##_list_n                                                         \
  {                                                                           \
    PRE##_list_n *prev;                                                       \
    PRE##_list_n *next;                                                       \
    DTYPE data;                                                               \
  };                                                                          \
                                                                              \
  static const list_i PRE##_list_info                                         \
      = { .d_size = sizeof (DTYPE),                                           \
          .n_size = sizeof (PRE##_list),                                      \
          .d_offs = offsetof (PRE##_list_n, data) };                          \
                                                                              \
  static void PRE##_list_init (PRE##_list *lis)                               \
      __attribute__ ((nonnull (1)));                                          \
                                                                              \
  static void PRE##_list_free (PRE##_list *lis)                               \
      __attribute__ ((nonnull (1)));                                          \
                                                                              \
  static void PRE##_list_remove (PRE##_list *lis, PRE##_list_n *pos)          \
      __attribute__ ((nonnull (1, 2)));                                       \
                                                                              \
  static PRE##_list_n *PRE##_list_at (const PRE##_list *lis, size_t pos)      \
      __attribute__ ((nonnull (1)));                                          \
                                                                              \
  static PRE##_list_n *PRE##_list_push_back (PRE##_list *lis, DTYPE data)     \
      __attribute__ ((nonnull (1)));                                          \
                                                                              \
  static PRE##_list_n *PRE##_list_push_front (PRE##_list *lis, DTYPE data)    \
      __attribute__ ((nonnull (1)));                                          \
                                                                              \
  static PRE##_list_n *PRE##_list_insert (PRE##_list *lis, PRE##_list_n *pos, \
                                          DTYPE data)                         \
      __attribute__ ((nonnull (1, 2)));

#define LIST_DEF_ALL(PRE, DTYPE)                                              \
  LIST_DEF_INFO (PRE, DTYPE)                                                  \
                                                                              \
  LIST_DEF_AT (PRE, DTYPE)                                                    \
  LIST_DEF_INIT (PRE, DTYPE)                                                  \
  LIST_DEF_FREE (PRE, DTYPE)                                                  \
  LIST_DEF_INSERT (PRE, DTYPE)                                                \
  LIST_DEF_REMOVE (PRE, DTYPE)                                                \
  LIST_DEF_PUSH_BACK (PRE, DTYPE)                                             \
  LIST_DEF_PUSH_FRONT (PRE, DTYPE)

#endif
