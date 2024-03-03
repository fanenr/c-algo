#include "avlmap.h"
#include <alloca.h>
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
  *map = (avlmap){ .size = 0, .root = NULL };
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

static inline avlmap_height_t
height_update (avlmap_n *node)
{
  avlmap_height_t lh = HEIGHT_OF (node->left);
  avlmap_height_t rh = HEIGHT_OF (node->right);
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
avlmap_swap_data (avlmap_n *node1, avlmap_n *node2, const avlmap_i *info)
{
  void *ktemp = NULL;
  void *kpos1 = KEY_OF (node1, info);
  void *kpos2 = KEY_OF (node2, info);
  size_t d_size = info->n_size - info->k_offs;

  if (!(ktemp = alloca (d_size)))
    return false;
  if (memcpy (ktemp, kpos1, d_size) != ktemp)
    return false;
  if (memcpy (kpos1, kpos2, d_size) != kpos1)
    return false;
  if (memcpy (kpos2, ktemp, d_size) != kpos2)
    return false;

  return true;
}

void
avlmap_remove (avlmap *map, void *key, const avlmap_i *info)
{
  avlmap_height_t num = 0;
  avlmap_n *parents[HEIGHT_MAX];
  avlmap_height_t heights[HEIGHT_MAX];

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
    return;

  if (!node->left && !node->right)
    {
      *rmpos = NULL;
      free (node);
      goto balance;
    }

  if (!node->left ^ !node->right)
    {
      *rmpos = node->left ?: node->right;
      free (node);
      goto balance;
    }

  parents[num] = node;
  heights[num++] = node->height;

  avlmap_n *old = node, *left;
  node = *(rmpos = &node->right);
  while ((left = node->left))
    {
      parents[num] = node;
      heights[num++] = node->height;
      node = *(rmpos = &node->left);
    }

  if (!avlmap_swap_data (old, node, info))
    return;

  *rmpos = node->right;
  free (node);

balance:
  for (; num; num--)
    {
      avlmap_n *curr = parents[num - 1];
      avlmap_height_t height = heights[num - 1];

      avlmap_height_t updated = height_update (curr);
      avlmap_n *rotated = rotate (curr);

      if (rotated == curr)
        {
          if (updated == height)
            break;
          continue;
        }

      avlmap_n **repos = &map->root;
      if (num > 1)
        {
          avlmap_n *father = parents[num - 2];
          repos = (curr == father->left) ? &father->left : &father->right;
        }
      *repos = rotated;
    }

  map->size--;
  return;
}

avlmap_n *
avlmap_find (const avlmap *map, void *key, const avlmap_i *info)
{
  if (!map->size)
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

static inline avlmap_n *
avlmap_node_new (void *key, void *val, const avlmap_i *info)
{
  avlmap_n *node = NULL;
  if (!(node = malloc (info->n_size)))
    return NULL;

  node->height = 0;
  node->left = node->right = NULL;

  void *nkey = KEY_OF (node, info);
  void *nval = VAL_OF (node, info);
  if (memcpy (nkey, key, info->k_size) != nkey)
    goto error;
  if (memcpy (nval, val, info->v_size) != nval)
    goto error;

  return node;

error:
  free (node);
  return NULL;
}

avlmap_n *
avlmap_insert (avlmap *map, void *key, void *val, const avlmap_i *info)
{
  avlmap_height_t num = 0;
  avlmap_n *parents[HEIGHT_MAX];
  avlmap_height_t heights[HEIGHT_MAX];

  avlmap_n *node = NULL;
  avlmap_n **inpos = &map->root;

  for (avlmap_n *curr = map->root; curr;)
    {
      void *ckey = KEY_OF (curr, info);
      int comp = info->f_comp (key, ckey);

      if (comp == 0)
        return NULL;

      parents[num] = curr;
      heights[num++] = curr->height;
      curr = *(inpos = comp < 0 ? &curr->left : &curr->right);
    }

  if (!(node = avlmap_node_new (key, val, info)))
    return NULL;

  *inpos = node;

  for (; num; num--)
    {
      avlmap_n *curr = parents[num - 1];
      if (heights[num - 1] == height_update (curr))
        break;

      avlmap_n *rotated = rotate (curr);
      if (rotated == curr)
        continue;

      avlmap_n **repos = &map->root;
      if (num > 1)
        {
          avlmap_n *father = parents[num - 2];
          repos = (curr == father->left) ? &father->left : &father->right;
        }
      *repos = rotated;
    }

  map->size++;
  return node;
}
