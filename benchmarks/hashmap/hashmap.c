#include "hashmap.h"
#include <stdlib.h>
#include <string.h>

#define INDEX_AT(MAP, HASH) ((HASH) % (MAP)->cap)
#define INDEX_NEXT(INDEX, TIME) ((INDEX) + (TIME))
#define NODE_AT(MAP, INDEX, INFO)                                             \
  ((void *)(MAP)->data + (INDEX) * (INFO)->n_size)

#define KEY_OF(NODE, INFO) ((void *)(NODE) + (INFO)->k_offs)
#define VAL_OF(NODE, INFO) ((void *)(NODE) + (INFO)->v_offs)

void
hashmap_init (hashmap *map)
{
  *map = (hashmap){ .cap = 0, .size = 0, .data = NULL };
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

  hashmap_n *data;
  if (!(data = calloc (cap, info->n_size)))
    return NULL;

  hashmap nmap = { .cap = cap, .size = 0, .data = data };
  for (size_t i = 0; i < map->cap; i++)
    {
      hashmap_n *node = NODE_AT (map, i, info);
      void *nkey = KEY_OF (node, info);
      void *nval = VAL_OF (node, info);

      if (node->sts != HASHMAP_STATE_USED)
        continue;

      if (!hashmap_insert (&nmap, nkey, nval, info))
        {
          free (data);
          return NULL;
        }
    }

  free (map->data);
  map->data = data;
  map->cap = cap;
  return map;
}

hashmap_n *
hashmap_find (const hashmap *map, void *key, const hashmap_i *info)
{
  if (!map->cap || !map->size)
    return NULL;

  size_t hash = info->f_hash (key);
  size_t index = INDEX_AT (map, hash);

  hashmap_comp_t *const f_comp = info->f_comp;
  for (size_t i = 1;; i++)
    {
      hashmap_n *node = NODE_AT (map, index, info);
      hashmap_state_t sts = node->sts;

      if (sts == HASHMAP_STATE_EMPTY)
        return NULL;

      if (sts == HASHMAP_STATE_USED)
        {
          size_t nhash = node->hash;
          void *nkey = KEY_OF (node, info);
          if (hash == nhash && f_comp (key, nkey) == 0)
            return node;
        }

      index = INDEX_NEXT (index, i);
      if (index >= map->cap)
        return NULL;
    }
}

void
hashmap_remove (hashmap *map, void *key, const hashmap_i *info)
{
  hashmap_n *node;
  if (!(node = hashmap_find (map, key, info)))
    return;

  node->sts = HASHMAP_STATE_RMED;
  map->size--;
}

hashmap_n *
hashmap_insert (hashmap *map, void *key, void *val, const hashmap_i *info)
{
  if (map->size >= map->cap * HASHMAP_LOAD_FACTOR)
    {
      size_t ncap = map->cap * HASHMAP_EXPAN_RATIO;
      ncap < HASHMAP_INIT_CAP ? ncap = HASHMAP_INIT_CAP : 0;
      if (!hashmap_reserve (map, ncap, info))
        return NULL;
    }

  hashmap_n *node;
  size_t hash = info->f_hash (key);
  size_t index = INDEX_AT (map, hash);

  hashmap_comp_t *const f_comp = info->f_comp;
  for (size_t i = 1;; i++)
    {
      node = NODE_AT (map, index, info);

      if (node->sts != HASHMAP_STATE_USED)
        break;

      size_t nhash = node->hash;
      void *nkey = KEY_OF (node, info);
      if (hash == nhash && f_comp (key, nkey) == 0)
        return NULL;

      index = INDEX_NEXT (index, i);
      if (index < map->cap)
        continue;

      size_t ncap = map->cap * HASHMAP_EXPAN_RATIO;
      if (!hashmap_reserve (map, ncap, info))
        return NULL;

      index = INDEX_AT (map, hash);
      i = 0;
    }

  node->hash = hash;
  node->sts = HASHMAP_STATE_USED;

  void *nkey = KEY_OF (node, info);
  void *nval = VAL_OF (node, info);
  if (memcpy (nkey, key, info->k_size) != nkey)
    return NULL;
  if (memcpy (nval, val, info->v_size) != nval)
    return NULL;

  map->size++;
  return node;
}
