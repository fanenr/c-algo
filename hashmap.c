#include "hashmap.h"
#include <stdlib.h>
#include <string.h>

static inline size_t
next_index (size_t index, size_t i)
{
  return index + i * i;
}

void
hashmap_init (hashmap *map)
{
  *map = (hashmap){ .cap = 0, .len = 0, .data = NULL };
}

void
hashmap_free (hashmap *map)
{
  free (map->data);
  hashmap_init (map);
}

hashmap *
hashmap_reserve (hashmap *map, size_t cap, const hashmap_i *info)
{
  if (map->cap >= cap)
    return map;

  size_t ncap = map->cap;
  if (ncap < HASHMAP_INIT_CAP)
    ncap = HASHMAP_INIT_CAP;

  while (ncap < cap)
    ncap *= HASHMAP_EXPAN_RATIO;

  void *ndat = calloc (ncap, info->n_size);
  if (ndat == NULL)
    return NULL;

  hashmap nmap = { .len = 0, .cap = ncap, .data = ndat };
  for (size_t i = 0; i < map->cap; i++)
    {
      void *node = map->data + i * info->n_size;
      char sts = *(char *)node;
      void *key = node + info->k_offs;
      void *val = node + info->v_offs;
      if (sts == HASHMAP_STATE_EMPTY || sts == HASHMAP_STATE_RMED)
        break;

      /* sts = HASHMAP_STATE_USED */
      if (!hashmap_insert (&nmap, key, val, info))
        return NULL;
    }

  free (map->data);
  map->data = ndat;
  map->cap = ncap;
  return map;
}

void *
hashmap_find (hashmap *map, void *key, const hashmap_i *info)
{
  if (!map->cap || !map->len)
    return NULL;

  size_t hash = info->f_hash (key);
  size_t index = hash % map->cap;
  void *node = map->data + index * info->n_size;

  void *nkey;
  for (size_t i = 1;; node = map->data + index * info->n_size, i++)
    switch (*(char *)node)
      {
      case HASHMAP_STATE_EMPTY:
        return NULL;

      case HASHMAP_STATE_RMED:
        if ((index = next_index (index, i)) >= map->cap)
          return NULL;
        break;

      case HASHMAP_STATE_USED:
        nkey = node + info->k_offs;
        if (info->f_comp (key, nkey) == 0)
          return node;
        if ((index = next_index (index, i)) >= map->cap)
          return NULL;
        break;
      }
}

void
hashmap_remove (hashmap *map, void *key, const hashmap_i *info)
{
  void *node = hashmap_find (map, key, info);
  if (!node)
    return;

  *(char *)node = HASHMAP_STATE_RMED;
  map->len--;
}

void *
hashmap_insert (hashmap *map, void *key, void *val, const hashmap_i *info)
{
  if (map->len >= map->cap * HASHMAP_LOAD_FACTOR)
    {
      size_t ncap = map->cap * HASHMAP_EXPAN_RATIO;
      if (ncap < HASHMAP_INIT_CAP)
        ncap = HASHMAP_INIT_CAP;
      if (!hashmap_reserve (map, ncap, info))
        return NULL;
    }

  size_t hash = info->f_hash (key);
  size_t index = hash % map->cap;
  void *node = map->data + index * info->n_size;

  for (size_t i = 1;; node = map->data + index * info->n_size, i++)
    {
      char sts = *(char *)node;
      if (sts == HASHMAP_STATE_EMPTY || sts == HASHMAP_STATE_RMED)
        break;

      /* sts = HASHMAP_STATE_USED */
      void *nkey = node + info->k_offs;
      if (info->f_comp (key, nkey) == 0)
        return NULL;

      if ((index = next_index (index, i)) < map->cap)
        break;

      /* index >= map->cap */
      if (!hashmap_reserve (map, index + 1, info))
        return NULL;
    }

  void *nkey = node + info->k_offs;
  void *nval = node + info->v_offs;
  *(char *)node = HASHMAP_STATE_USED;

  if (memmove (nkey, key, info->k_size) != nkey)
    return NULL;

  if (memmove (nval, val, info->v_size) != nval)
    return NULL;

  map->len++;
  return node;
}
