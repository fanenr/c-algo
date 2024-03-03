#ifndef RBMAP_H
#define RBMAP_H

#include <stddef.h>

typedef int rbmap_color_t;
typedef struct rbmap rbmap;
typedef struct rbmap_i rbmap_i;
typedef struct rbmap_n rbmap_n;
typedef int rbmap_comp_t (const void *key1, const void *key2);

struct rbmap_i
{
  size_t k_size;
  size_t v_size;
  size_t n_size;
  size_t k_offs;
  size_t v_offs;
  rbmap_comp_t *f_comp;
};

#define RBMAP_RED 0
#define RBMAP_BLACK 1

struct rbmap_n
{
  rbmap_n *left;
  rbmap_n *right;
  rbmap_n *parent;
  rbmap_color_t color;
};

struct rbmap
{
  size_t size;
  rbmap_n *root;
};

extern void rbmap_init (rbmap *map);

extern void rbmap_free (rbmap *map);

extern void rbmap_remove (rbmap *map, void *key, const rbmap_i *info);

extern rbmap_n *rbmap_find (const rbmap *map, void *key, const rbmap_i *info);

extern rbmap_n *rbmap_insert (rbmap *map, void *key, void *val,
                              const rbmap_i *info);

#define RBMAP_DEF_REMOVE(PRE, KTYPE, VTYPE)                                   \
  static inline void PRE##_rbmap_remove (rbmap *map, KTYPE key)               \
  {                                                                           \
    rbmap_remove (map, &key, &PRE##_rbmap_info);                              \
  }

#define RBMAP_DEF_FIND(PRE, KTYPE, VTYPE)                                     \
  static inline PRE##_rbmap_n *PRE##_rbmap_find (const rbmap *map, KTYPE key) \
  {                                                                           \
    return (PRE##_rbmap_n *)rbmap_find (map, &key, &PRE##_rbmap_info);        \
  }

#define RBMAP_DEF_INSERT(PRE, KTYPE, VTYPE)                                   \
  static inline PRE##_rbmap_n *PRE##_rbmap_insert (rbmap *map, KTYPE key,     \
                                                   VTYPE val)                 \
  {                                                                           \
    return (PRE##_rbmap_n *)rbmap_insert (map, &key, &val,                    \
                                          &PRE##_rbmap_info);                 \
  }

#define RBMAP_DEF_INFO(PRE, KTYPE, VTYPE, COMP)                               \
  typedef struct PRE##_rbmap_n PRE##_rbmap_n;                                 \
                                                                              \
  struct PRE##_rbmap_n                                                        \
  {                                                                           \
    rbmap_n *left;                                                            \
    rbmap_n *right;                                                           \
    rbmap_n *parent;                                                          \
    rbmap_color_t color;                                                      \
    KTYPE key;                                                                \
    VTYPE val;                                                                \
  };                                                                          \
                                                                              \
  static const rbmap_i PRE##_rbmap_info                                       \
      = { .k_size = sizeof (KTYPE),                                           \
          .v_size = sizeof (VTYPE),                                           \
          .n_size = sizeof (PRE##_rbmap_n),                                   \
          .k_offs = offsetof (PRE##_rbmap_n, key),                            \
          .v_offs = offsetof (PRE##_rbmap_n, val),                            \
          .f_comp = (rbmap_comp_t *)COMP };                                   \
                                                                              \
  static void PRE##_rbmap_remove (rbmap *map, KTYPE key)                      \
      __attribute__ ((nonnull (1)));                                          \
                                                                              \
  static PRE##_rbmap_n *PRE##_rbmap_find (const rbmap *map, KTYPE key)        \
      __attribute__ ((nonnull (1)));                                          \
                                                                              \
  static PRE##_rbmap_n *PRE##_rbmap_insert (rbmap *map, KTYPE key, VTYPE val) \
      __attribute__ ((nonnull (1)));

#define RBMAP_DEF_ALL(PRE, KTYPE, VTYPE, COMP)                                \
  RBMAP_DEF_INFO (PRE, KTYPE, VTYPE, COMP);                                   \
  RBMAP_DEF_REMOVE (PRE, KTYPE, VTYPE);                                       \
  RBMAP_DEF_FIND (PRE, KTYPE, VTYPE);                                         \
  RBMAP_DEF_INSERT (PRE, KTYPE, VTYPE)

#endif
