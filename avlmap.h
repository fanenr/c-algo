#ifndef AVLMAP_H
#define AVLMAP_H

#include <stddef.h>

typedef int avlmap_comp_t (const void *key1, const void *key2);

typedef struct avlmap_i
{
  size_t k_size;
  size_t v_size;
  size_t n_size;
  size_t k_offs;
  size_t v_offs;
  avlmap_comp_t *f_comp;
} avlmap_i;

typedef struct avlmap_n avlmap_n;

struct avlmap_n
{
  int height;
  avlmap_n *left;
  avlmap_n *right;
};

typedef struct avlmap
{
  size_t len;
  avlmap_n *root;
} avlmap;

extern void avlmap_init (avlmap *map);

extern void avlmap_free (avlmap *map);

extern void avlmap_remove (avlmap *map, void *key, const avlmap_i *info)
    __attribute__ ((nonnull (1, 2, 3)));

extern avlmap_n *avlmap_find (const avlmap *map, void *key,
                              const avlmap_i *info)
    __attribute__ ((nonnull (1, 2, 3)));

extern avlmap_n *avlmap_insert (avlmap *map, void *key, void *val,
                                const avlmap_i *info)
    __attribute__ ((nonnull (1, 2, 3, 4)));

#define AVLMAP_DEF_REMOVE(KTYPE, VTYPE, PRE)                                  \
  static inline void PRE##_avlmap_remove (avlmap *map, KTYPE key)             \
  {                                                                           \
    avlmap_remove (map, &key, &PRE##_avlmap_info);                            \
  }

#define AVLMAP_DEF_FIND(KTYPE, VTYPE, PRE)                                    \
  static inline PRE##_avlmap_n *PRE##_avlmap_find (const avlmap *map,         \
                                                   KTYPE key)                 \
  {                                                                           \
    return (PRE##_avlmap_n *)avlmap_find (map, &key, &PRE##_avlmap_info);     \
  }

#define AVLMAP_DEF_INSERT(KTYPE, VTYPE, PRE)                                  \
  static inline PRE##_avlmap_n *PRE##_avlmap_insert (avlmap *map, KTYPE key,  \
                                                     VTYPE val)               \
  {                                                                           \
    return (PRE##_avlmap_n *)avlmap_insert (map, &key, &val,                  \
                                            &PRE##_avlmap_info);              \
  }

#define AVLMAP_DEF_INFO(KTYPE, VTYPE, PRE)                                    \
  typedef struct PRE##_avlmap_n PRE##_avlmap_n;                               \
                                                                              \
  struct PRE##_avlmap_n                                                       \
  {                                                                           \
    int height;                                                               \
    avlmap_n *left;                                                           \
    avlmap_n *right;                                                          \
    KTYPE key;                                                                \
    VTYPE val;                                                                \
  };                                                                          \
                                                                              \
  static const avlmap_i PRE##_avlmap_info                                     \
      = { .k_size = sizeof (KTYPE),                                           \
          .v_size = sizeof (VTYPE),                                           \
          .n_size = sizeof (PRE##_avlmap_n),                                  \
          .k_offs = offsetof (PRE##_avlmap_n, key),                           \
          .v_offs = offsetof (PRE##_avlmap_n, val),                           \
          .f_comp = (avlmap_comp_t *)PRE##_avlmap_comp };                     \
                                                                              \
  static void PRE##_avlmap_remove (avlmap *map, KTYPE key)                    \
      __attribute__ ((nonnull (1)));                                          \
                                                                              \
  static PRE##_avlmap_n *PRE##_avlmap_find (const avlmap *map, KTYPE key)     \
      __attribute__ ((nonnull (1)));                                          \
                                                                              \
  static PRE##_avlmap_n *PRE##_avlmap_insert (                                \
      avlmap *map, KTYPE key, VTYPE val) __attribute__ ((nonnull (1)));

#define AVLMAP_DEF_ALL(KTYPE, VTYPE, PRE)                                     \
  AVLMAP_DEF_INFO (KTYPE, VTYPE, PRE);                                        \
  AVLMAP_DEF_REMOVE (KTYPE, VTYPE, PRE);                                      \
  AVLMAP_DEF_FIND (KTYPE, VTYPE, PRE);                                        \
  AVLMAP_DEF_INSERT (KTYPE, VTYPE, PRE)

#endif
