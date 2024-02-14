#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdbool.h>
#include <stddef.h>

#define HASHMAP_INIT_CAP 8
#define HASHMAP_EXPAN_RATIO 2
#define HASHMAP_LOAD_FACTOR 0.8

#define HASHMAP_STATE_USED 2
#define HASHMAP_STATE_RMED 1
#define HASHMAP_STATE_EMPTY 0

typedef long hashmap_hash_t (void *key);
typedef int hashmap_comp_t (void *key1, void *key2);

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

extern void hashmap_init (hashmap *map);

extern void hashmap_free (hashmap *map);

extern void hashmap_remove (hashmap *map, void *key, const hashmap_i *info);

extern hashmap *hashmap_reserve (hashmap *map, size_t cap,
                                 const hashmap_i *info);

extern void *hashmap_find (hashmap *map, void *key, const hashmap_i *info);

extern void *hashmap_insert (hashmap *map, void *key, void *val,
                             const hashmap_i *info);

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
          .f_comp = (hashmap_comp_t *)PRE##_hashmap_comp }

#endif