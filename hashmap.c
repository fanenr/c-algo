#include "hashmap.h"
#include <stdalign.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static inline long
next_hash (hashmap *map, long hash)
{
  long ret = hash * hash;
  if ((size_t)ret >= map->cap)
    return -1;
  return ret;
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

  void *ndat = realloc (map->data, ncap * info->n_size);
  if (ndat == NULL)
    return NULL;

  for (size_t i = map->cap; i < ncap; i++)
    {
      char *state = map->data + info->n_size * i;
      *state = HASHMAP_STATE_EMPTY;
    }

  map->data = ndat;
  map->cap = ncap;
  return map;
}

void *
hashmap_find (hashmap *map, void *key, const hashmap_i *info)
{
  if (!map->cap || !map->len)
    return NULL;

  long code = info->f_hash (key);
  if (code < 0)
    return NULL;

  long hash = code % map->cap;
  void *node = map->data + hash * info->n_size;

  for (; node; node = map->data + hash * info->n_size)
    {
      char state = *(char *)node;
      if (state == HASHMAP_STATE_EMPTY)
        break;

      if (state == HASHMAP_STATE_RMED)
        if ((hash = next_hash (map, hash)) < 0)
          break;

      if (state == HASHMAP_STATE_USED)
        {
          void *nkey = node + info->k_offs;
          if (info->f_comp (key, nkey) == 0)
            return node;
          if ((hash = next_hash (map, hash)) < 0)
            break;
        }
    }

  return NULL;
}

void
hashmap_remove (hashmap *map, void *key, const hashmap_i *info)
{
  void *node = hashmap_find (map, key, info);
  if (!node)
    return;

  char *state = (char *)node;
  *state = HASHMAP_STATE_RMED;
  map->len--;
}

void *
hashmap_insert (hashmap *map, void *key, void *val, const hashmap_i *info)
{
  if (map->len * HASHMAP_LOAD_FACTOR >= map->cap)
    if (!hashmap_reserve (map, map->cap * HASHMAP_EXPAN_RATIO, info))
      return NULL;

  long code = info->f_hash (key);
  if (code < 0)
    return NULL;

  long hash = code % map->cap;
  void *node = map->data + hash * info->n_size;

  for (;; node = map->data + hash * info->n_size)
    {
      char state = *(char *)node;
      if (state == HASHMAP_STATE_EMPTY)
        break;

      if (state == HASHMAP_STATE_RMED)
        break;

      if (state == HASHMAP_STATE_USED)
        {
          void *nkey = node + info->k_offs;
          if (info->f_comp (key, nkey) == 0)
            return NULL;

          for (; hash < 0; hash = next_hash (map, hash))
            if (!hashmap_reserve (map, map->cap * HASHMAP_EXPAN_RATIO, info))
              return NULL;
        }
    }

  char *state = (char *)node;
  *state = HASHMAP_STATE_USED;
  void *nkey = node + info->k_offs;
  void *nval = node + info->v_offs;

  if (memmove (nkey, key, info->k_size) != nkey)
    return NULL;

  if (memmove (nval, val, info->v_size) != nval)
    return NULL;

  map->len++;
  return node;
}
