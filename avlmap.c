#include "avlmap.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define HEIGHT_MAX 36

#define HEIGHT_OF(NODE) ((NODE) ? (NODE)->height : -1)
#define BALANCE_FACTOR_OF(NODE)                                               \
  ((NODE) ? HEIGHT_OF ((NODE)->left) - HEIGHT_OF ((NODE)->right) : 0)

#define KEY_OF(NODE, INFO) ((void *)(NODE) + (INFO)->k_offs)
#define VAL_OF(NODE, INFO) ((void *)(NODE) + (INFO)->v_offs)

void
avlmap_init (avlmap *map)
{
  *map = (avlmap){ .len = 0, .root = NULL };
}

static inline void
avlmap_free_impl (avlmap_n *node)
{
  if (!node)
    return;
  avlmap_free_impl (node->left);
  avlmap_free_impl (node->right);
  free (node);
}

void
avlmap_free (avlmap *map)
{
  avlmap_free_impl (map->root);
  avlmap_init (map);
}

static inline signed char
height_update (avlmap_n *node)
{
  signed char lh = HEIGHT_OF (node->left);
  signed char rh = HEIGHT_OF (node->right);
  return (node->height = (lh > rh ? lh : rh) + 1);
}

static inline avlmap_n *
rotate_left (avlmap_n *node)
{
  avlmap_n *child = node->right;

  node->right = child->left;
  child->left = node;

  height_update (node);
  height_update (child);
  return child;
}

static inline avlmap_n *
rotate_right (avlmap_n *node)
{
  avlmap_n *child = node->left;

  node->left = child->right;
  child->right = node;

  height_update (node);
  height_update (child);
  return child;
}

static inline avlmap_n *
rotate (avlmap_n *node)
{
  int bf = BALANCE_FACTOR_OF (node);

  if (bf > 1)
    {
      if (BALANCE_FACTOR_OF (node->left) >= 0)
        return rotate_right (node);

      node->left = rotate_left (node->left);
      return rotate_right (node);
    }

  if (bf < -1)
    {
      if (BALANCE_FACTOR_OF (node->right) <= 0)
        return rotate_left (node);

      node->right = rotate_right (node->right);
      return rotate_left (node);
    }

  return node;
}

static inline bool
avlmap_swap_data (void *kpos1, void *kpos2, const avlmap_i *info)
{
  void *bakup = NULL;
  size_t d_size = info->n_size - info->k_offs;

  if (!(bakup = malloc (d_size)))
    return false;
  if (memcpy (bakup, kpos1, d_size) != bakup)
    goto error;
  if (memcpy (kpos1, kpos2, d_size) != kpos1)
    goto error;
  if (memcpy (kpos2, bakup, d_size) != kpos2)
    goto error;

  free (bakup);
  return true;

error:
  free (bakup);
  return false;
}

void
avlmap_remove (avlmap *map, void *key, const avlmap_i *info)
{
  unsigned num = 0;
  avlmap_n *parents[HEIGHT_MAX];
  signed char heights[HEIGHT_MAX];
  avlmap_n *node = NULL;
  avlmap_n **rmpos = &map->root;

  for (avlmap_n *curr = map->root; curr;)
    {
      void *ckey = KEY_OF (curr, info);
      int comp = info->f_comp (key, ckey);

      if (comp == 0)
        {
          node = curr;
          break;
        }

      parents[num] = curr;
      heights[num++] = curr->height;
      curr = *(rmpos = comp < 0 ? &curr->left : &curr->right);
    }

  if (!node)
    /* not found */
    return;

  if (!node->left && !node->right)
    { /* no children */
      *rmpos = NULL;
      free (node);
      map->len--;
      goto balance;
    }

  if (!node->left ^ !node->right)
    { /* one children */
      *rmpos = node->left ?: node->right;
      free (node);
      map->len--;
      goto balance;
    }

  /* two children */
  parents[num] = node;
  heights[num++] = node->height;
  avlmap_n *node_bak = node;
  node = *(rmpos = &node->right);
  while (node->left)
    {
      parents[num] = node;
      heights[num++] = node->height;
      node = *(rmpos = &node->left);
    }

  /* swap data */
  void *nkey = KEY_OF (node_bak, info);
  void *bkey = KEY_OF (node, info);
  if (!avlmap_swap_data (nkey, bkey, info))
    return;

  *rmpos = node->left ?: node->right;
  free (node);
  map->len--;

balance:
  for (; num; num--)
    {
      avlmap_n *curr = parents[num - 1];
      signed char height = heights[num - 1];
      avlmap_n *rotated = rotate (curr);
      signed char updated = height_update (curr);

      if (updated == height && rotated == curr)
        break;

      avlmap_n **repos = &map->root;
      if (num - 1)
        {
          avlmap_n *father = parents[num - 2];
          repos = curr == father->left ? &father->left : &father->right;
        }
      *repos = rotated;
    }

  return;
}

avlmap_n *
avlmap_find (const avlmap *map, void *key, const avlmap_i *info)
{
  if (!map->len)
    return NULL;

  for (avlmap_n *curr = map->root; curr;)
    {
      void *ckey = KEY_OF (curr, info);
      int comp = info->f_comp (key, ckey);

      if (comp == 0)
        return curr;

      curr = comp < 0 ? curr->left : curr->right;
    }

  return NULL;
}

avlmap_n *
avlmap_insert (avlmap *map, void *key, void *val, const avlmap_i *info)
{
  unsigned num = 0;
  avlmap_n *parents[HEIGHT_MAX];
  signed char heights[HEIGHT_MAX];
  avlmap_n **inpos = &map->root;

  for (avlmap_n *curr = map->root; curr;)
    {
      void *ckey = KEY_OF (curr, info);
      int comp = info->f_comp (key, ckey);

      if (comp == 0)
        return curr;

      parents[num] = curr;
      heights[num++] = curr->height;
      curr = *(inpos = comp < 0 ? &curr->left : &curr->right);
    }

  /* allocate node and initialize */
  avlmap_n *node = malloc (info->n_size);
  void *nkey = KEY_OF (node, info);
  void *nval = VAL_OF (node, info);

  node->height = 0;
  node->left = node->right = NULL;
  if (memcpy (nkey, key, info->k_size) != nkey)
    goto error;
  if (memcpy (nval, val, info->v_size) != nval)
    goto error;

  /* insert node */
  *inpos = node;
  map->len++;

  /* rebalance */
  if (num > 2)
    for (; num; num--)
      {
        avlmap_n *curr = parents[num - 1];
        if (heights[num - 1] == height_update (curr))
          break;

        avlmap_n *rotated = rotate (curr);
        if (rotated == curr)
          continue;

        avlmap_n **repos = &map->root;
        if (num - 1)
          {
            avlmap_n *father = parents[num - 2];
            repos = curr == father->left ? &father->left : &father->right;
          }
        *repos = rotated;
      }

  return node;

error:
  free (node);
  return NULL;
}
