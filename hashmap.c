#include "hashmap.h"
#include <stdlib.h>
#include <string.h>

#define NEXT_INDEX(INDEX, TIME) ((INDEX) + (TIME) * (TIME))
#define NODE_AT(MAP, INDEX, INFO) ((MAP)->data + (INDEX) * (INFO)->n_size)

#define STS_OF(NODE) ((char *)NODE)
#define KEY_OF(NODE, INFO) ((void *)(NODE) + (INFO)->k_offs)
#define VAL_OF(NODE, INFO) ((void *)(NODE) + (INFO)->v_offs)

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
      void *node = NODE_AT (map, i, info);
      void *key = KEY_OF (node, info);
      void *val = VAL_OF (node, info);

      char sts = *STS_OF (node);
      if (sts == HASHMAP_STATE_EMPTY || sts == HASHMAP_STATE_RMED)
        break;

      /* sts = HASHMAP_STATE_USED */
      if (!hashmap_insert (&nmap, key, val, info))
        {
          free (ndat);
          return NULL;
        }
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

  for (size_t i = 1;; i++)
    {
      void *node = NODE_AT (map, index, info);
      char sts = *STS_OF (node);

      if (sts == HASHMAP_STATE_EMPTY)
        return NULL;

      if (sts == HASHMAP_STATE_RMED)
        {
          index = NEXT_INDEX (index, i);
          if (index < map->cap)
            continue;
          return NULL;
        }

      void *nkey = KEY_OF (node, info);
      if (sts == HASHMAP_STATE_USED)
        {
          if (info->f_comp (key, nkey) == 0)
            return node;

          index = NEXT_INDEX (index, i);
          if (index < map->cap)
            continue;
          return NULL;
        }
    }
}

void
hashmap_remove (hashmap *map, void *key, const hashmap_i *info)
{
  void *node = hashmap_find (map, key, info);
  if (!node)
    return;

  *STS_OF (node) = HASHMAP_STATE_RMED;
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

  void *node;
  size_t hash = info->f_hash (key);
  size_t index = hash % map->cap;

  for (size_t i = 1;; i++)
    {
      node = NODE_AT (map, index, info);
      char sts = *STS_OF (node);

      if (sts == HASHMAP_STATE_EMPTY || sts == HASHMAP_STATE_RMED)
        break;

      void *nkey = KEY_OF (node, info);
      if (info->f_comp (key, nkey) == 0)
        return NULL;

      index = NEXT_INDEX (index, i);
      if (index < map->cap)
        continue;

      if (!hashmap_reserve (map, index + 1, info))
        return NULL;
    }

  void *nkey = KEY_OF (node, info);
  void *nval = VAL_OF (node, info);
  *STS_OF (node) = HASHMAP_STATE_USED;

  if (memmove (nkey, key, info->k_size) != nkey)
    return NULL;

  if (memmove (nval, val, info->v_size) != nval)
    return NULL;

  map->len++;
  return node;
}
