#include "hashmap.h"
#include <stdalign.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static inline size_t
node_size (const hashmap_i *info)
{
  const size_t m_align
      = info->k_align > info->v_align ? info->k_align : info->v_align;
  size_t ret = sizeof (char) + info->k_size + info->v_size;
  if (ret % m_align)
    ret += m_align - ret % m_align;
  return ret;
}

static inline size_t
key_offset (const hashmap_i *info)
{
  size_t ret = 0 + sizeof (char);
  if (ret % info->k_align)
    ret += info->k_align - ret % info->k_align;
  return ret;
}

static inline size_t
val_offset (const hashmap_i *info)
{
  size_t ret = key_offset (info) + info->k_size;
  if (ret % info->v_align)
    ret += info->v_align - ret % info->v_align;
  return ret;
}

static inline void *
node_ptr (hashmap *map, long hash, const hashmap_i *info)
{
  return map->data + hash * node_size (info);
}

static inline void *
key_ptr (void *node, const hashmap_i *info)
{
  return node + key_offset (info);
}

static inline void *
val_ptr (void *node, const hashmap_i *info)
{
  return node + val_offset (info);
}

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

  void *ndat = realloc (map->data, ncap * node_size (info));
  if (ndat == NULL)
    return NULL;

  for (size_t i = map->cap; i < ncap; i++)
    {
      hashmap_n *node = get_node (map, i, k_ele, v_ele);
      node->state = HASHMAP_STATE_EMPTY;
    }

  map->data = ndat;
  map->cap = ncap;
  return map;
}

hashmap_n *
hashmap_find (hashmap *map, hashmap_hash_t *f_hash, hashmap_comp_t *f_comp,
              void *key, size_t k_ele, size_t v_ele)
{
  if (!map->cap || !map->len)
    return NULL;

  long code = f_hash (key);
  if (code < 0)
    return NULL;

  long hash = code % map->cap;
  hashmap_n *node = get_node (map, hash, k_ele, v_ele);

  for (; node; node = get_node (map, hash, k_ele, v_ele))
    {
      if (node->state == HASHMAP_STATE_EMPTY)
        break;

      if (node->state == HASHMAP_STATE_RMED)
        if ((hash = next_hash (map, hash)) < 0)
          break;

      if (node->state == HASHMAP_STATE_USED)
        {
          if (f_comp (key, node->key) == 0)
            return node;
          if ((hash = next_hash (map, hash)) < 0)
            break;
        }
    }

  return NULL;
}

void
hashmap_remove (hashmap *map, hashmap_hash_t *f_hash, hashmap_comp_t *f_comp,
                void *key, size_t k_ele, size_t v_ele)
{
  hashmap_n *node = hashmap_find (map, f_hash, f_comp, key, k_ele, v_ele);
  if (!node)
    return;

  node->state = HASHMAP_STATE_RMED;
  map->len--;
}

hashmap_n *
hashmap_insert (hashmap *map, hashmap_hash_t *f_hash, hashmap_comp_t *f_comp,
                void *key, void *val, size_t k_ele, size_t v_ele)
{
  if (map->len * HASHMAP_LOAD_FACTOR >= map->cap)
    if (!hashmap_reserve (map, map->cap * HASHMAP_EXPAN_RATIO, k_ele, v_ele))
      return NULL;

  long code = f_hash (key);
  if (code < 0)
    return NULL;

  long hash = code % map->cap;
  hashmap_n *node = get_node (map, hash, k_ele, v_ele);

  for (;; node = get_node (map, hash, k_ele, v_ele))
    {
      if (node->state == HASHMAP_STATE_EMPTY)
        break;

      if (node->state == HASHMAP_STATE_RMED)
        break;

      if (node->state == HASHMAP_STATE_USED)
        {
          if (f_comp (key, node->key) == 0)
            return NULL;

          for (; hash < 0; hash = next_hash (map, hash))
            if (!hashmap_reserve (map, map->cap * HASHMAP_EXPAN_RATIO, k_ele,
                                  v_ele))
              return NULL;
        }
    }

  node->state = HASHMAP_STATE_USED;
  node->val = (void *)node->key + aligned_size (k_ele, 0);

  if (memmove (node->key, key, k_ele) != node->key)
    return NULL;

  if (memmove (node->val, val, v_ele) != node->val)
    return NULL;

  map->len++;
  return node;
}
