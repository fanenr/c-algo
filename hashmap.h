#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdbool.h>
#include <stddef.h>

#define HASHMAP_INIT_CAP 8
#define HASHMAP_EXPAN_RATIO 2
#define HASHMAP_LOAD_FACTOR 0.75

#define HASHMAP_STATE_USED 2
#define HASHMAP_STATE_RMED 1
#define HASHMAP_STATE_EMPTY 0

typedef size_t hashmap_hash_t (const void *key);
typedef int hashmap_comp_t (const void *key1, const void *key2);

typedef struct hashmap_i
{
  size_t k_size;
  size_t v_size;
  size_t n_size;
  size_t k_offs;
  size_t v_offs;
  hashmap_hash_t *f_hash;
  hashmap_comp_t *f_comp;
} hashmap_i;

typedef struct hashmap
{
  size_t cap;
  size_t len;
  void *data;
} hashmap;

extern void hashmap_init (hashmap *map) __attribute__ ((nonnull (1)));

extern void hashmap_free (hashmap *map) __attribute__ ((nonnull (1)));

extern void hashmap_remove (hashmap *map, void *key, const hashmap_i *info)
    __attribute__ ((nonnull (1, 2, 3)));

extern hashmap *hashmap_reserve (hashmap *map, size_t cap,
                                 const hashmap_i *info)
    __attribute__ ((nonnull (1, 3)));

extern void *hashmap_find (const hashmap *map, void *key,
                           const hashmap_i *info)
    __attribute__ ((nonnull (1, 2, 3)));

extern void *hashmap_insert (hashmap *map, void *key, void *val,
                             const hashmap_i *info)
    __attribute__ ((nonnull (1, 2, 3, 4)));

#define HASHMAP_DEF_REMOVE(KTYPE, VTYPE, PRE)                                 \
  static inline void PRE##_hashmap_remove (hashmap *map, KTYPE key)           \
  {                                                                           \
    hashmap_remove (map, &key, &PRE##_hashmap_info);                          \
  }

#define HASHMAP_DEF_RESERVE(KTYPE, VTYPE, PRE)                                \
  static inline hashmap *PRE##_hashmap_reserve (hashmap *map, size_t cap)     \
  {                                                                           \
    return hashmap_reserve (map, cap, &PRE##_hashmap_info);                   \
  }

#define HASHMAP_DEF_FIND(KTYPE, VTYPE, PRE)                                   \
  static inline PRE##_hashmap_n *PRE##_hashmap_find (const hashmap *map,      \
                                                     KTYPE key)               \
  {                                                                           \
    return hashmap_find (map, &key, &PRE##_hashmap_info);                     \
  }

#define HASHMAP_DEF_INSERT(KTYPE, VTYPE, PRE)                                 \
  static inline PRE##_hashmap_n *PRE##_hashmap_insert (hashmap *map,          \
                                                       KTYPE key, VTYPE val)  \
  {                                                                           \
    return hashmap_insert (map, &key, &val, &PRE##_hashmap_info);             \
  }

#define HASHMAP_DEF_INFO(KTYPE, VTYPE, PRE)                                   \
  typedef struct PRE##_hashmap_n PRE##_hashmap_n;                             \
                                                                              \
  struct PRE##_hashmap_n                                                      \
  {                                                                           \
    char sts;                                                                 \
    KTYPE key;                                                                \
    VTYPE val;                                                                \
  };                                                                          \
                                                                              \
  static const hashmap_i PRE##_hashmap_info                                   \
      = { .k_size = sizeof (KTYPE),                                           \
          .v_size = sizeof (VTYPE),                                           \
          .n_size = sizeof (PRE##_hashmap_n),                                 \
          .k_offs = offsetof (PRE##_hashmap_n, key),                          \
          .v_offs = offsetof (PRE##_hashmap_n, val),                          \
          .f_hash = (hashmap_hash_t *)PRE##_hashmap_hash,                     \
          .f_comp = (hashmap_comp_t *)PRE##_hashmap_comp };                   \
                                                                              \
  static void PRE##_hashmap_remove (hashmap *map, KTYPE key)                  \
      __attribute__ ((nonnull (1)));                                          \
                                                                              \
  static hashmap *PRE##_hashmap_reserve (hashmap *map, size_t cap)            \
      __attribute__ ((nonnull (1)));                                          \
                                                                              \
  static PRE##_hashmap_n *PRE##_hashmap_find (const hashmap *map, KTYPE key)  \
      __attribute__ ((nonnull (1)));                                          \
                                                                              \
  static PRE##_hashmap_n *PRE##_hashmap_insert (                              \
      hashmap *map, KTYPE key, VTYPE val) __attribute__ ((nonnull (1)));

#define HASHMAP_DEF_ALL(KTYPE, VTYPE, PRE)                                    \
  HASHMAP_DEF_INFO (KTYPE, VTYPE, PRE);                                       \
  HASHMAP_DEF_REMOVE (KTYPE, VTYPE, PRE);                                     \
  HASHMAP_DEF_RESERVE (KTYPE, VTYPE, PRE);                                    \
  HASHMAP_DEF_FIND (KTYPE, VTYPE, PRE);                                       \
  HASHMAP_DEF_INSERT (KTYPE, VTYPE, PRE)

#endif