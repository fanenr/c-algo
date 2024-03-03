#ifndef HASHMAP_H
#define HASHMAP_H

#include <stddef.h>

#define HASHMAP_INIT_CAP 8
#define HASHMAP_EXPAN_RATIO 2
#define HASHMAP_LOAD_FACTOR 0.75

#define HASHMAP_STATE_USED 2
#define HASHMAP_STATE_RMED 1
#define HASHMAP_STATE_EMPTY 0

typedef char hashmap_state_t;
typedef struct hashmap hashmap;
typedef struct hashmap_i hashmap_i;
typedef struct hashmap_n hashmap_n;
typedef size_t hashmap_hash_t (const void *key);
typedef int hashmap_comp_t (const void *a, const void *b);

struct hashmap_i
{
  size_t k_size;
  size_t v_size;
  size_t n_size;
  size_t k_offs;
  size_t v_offs;
  hashmap_hash_t *f_hash;
  hashmap_comp_t *f_comp;
};

struct hashmap_n
{
  size_t hash;
  hashmap_state_t sts;
};

struct hashmap
{
  size_t cap;
  size_t size;
  hashmap_n *data;
};

extern void hashmap_init (hashmap *map) __attribute__ ((nonnull (1)));

extern void hashmap_free (hashmap *map) __attribute__ ((nonnull (1)));

extern void hashmap_remove (hashmap *map, void *key, const hashmap_i *info)
    __attribute__ ((nonnull (1, 2, 3)));

extern hashmap *hashmap_reserve (hashmap *map, size_t cap,
                                 const hashmap_i *info)
    __attribute__ ((nonnull (1, 3)));

extern hashmap_n *hashmap_find (const hashmap *map, void *key,
                                const hashmap_i *info)
    __attribute__ ((nonnull (1, 2, 3)));

extern hashmap_n *hashmap_insert (hashmap *map, void *key, void *val,
                                  const hashmap_i *info)
    __attribute__ ((nonnull (1, 2, 3, 4)));

#define HASHMAP_DEF_REMOVE(PRE, KTYPE, VTYPE)                                 \
  static inline void PRE##_hashmap_remove (hashmap *map, KTYPE key)           \
  {                                                                           \
    hashmap_remove (map, &key, &PRE##_hashmap_info);                          \
  }

#define HASHMAP_DEF_RESERVE(PRE, KTYPE, VTYPE)                                \
  static inline hashmap *PRE##_hashmap_reserve (hashmap *map, size_t cap)     \
  {                                                                           \
    return hashmap_reserve (map, cap, &PRE##_hashmap_info);                   \
  }

#define HASHMAP_DEF_FIND(PRE, KTYPE, VTYPE)                                   \
  static inline PRE##_hashmap_n *PRE##_hashmap_find (const hashmap *map,      \
                                                     KTYPE key)               \
  {                                                                           \
    return (PRE##_hashmap_n *)hashmap_find (map, &key, &PRE##_hashmap_info);  \
  }

#define HASHMAP_DEF_INSERT(PRE, KTYPE, VTYPE)                                 \
  static inline PRE##_hashmap_n *PRE##_hashmap_insert (hashmap *map,          \
                                                       KTYPE key, VTYPE val)  \
  {                                                                           \
    return (PRE##_hashmap_n *)hashmap_insert (map, &key, &val,                \
                                              &PRE##_hashmap_info);           \
  }

#define HASHMAP_DEF_INFO(PRE, KTYPE, VTYPE, HASH, COMP)                       \
  typedef struct PRE##_hashmap_n PRE##_hashmap_n;                             \
                                                                              \
  struct PRE##_hashmap_n                                                      \
  {                                                                           \
    size_t hash;                                                              \
    hashmap_state_t sts;                                                      \
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
          .f_hash = (hashmap_hash_t *)HASH,                                   \
          .f_comp = (hashmap_comp_t *)COMP };                                 \
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

#define HASHMAP_DEF_ALL(PRE, KTYPE, VTYPE, HASH, COMP)                        \
  HASHMAP_DEF_INFO (PRE, KTYPE, VTYPE, HASH, COMP);                           \
  HASHMAP_DEF_REMOVE (PRE, KTYPE, VTYPE);                                     \
  HASHMAP_DEF_RESERVE (PRE, KTYPE, VTYPE);                                    \
  HASHMAP_DEF_FIND (PRE, KTYPE, VTYPE);                                       \
  HASHMAP_DEF_INSERT (PRE, KTYPE, VTYPE)

#endif