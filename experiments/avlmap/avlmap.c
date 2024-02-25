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
  if (child->left)
    child->left->parent = node;
  child->parent = node->parent;
  node->parent = child;
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
  if (child->right)
    child->right->parent = node;
  child->parent = node->parent;
  node->parent = child;
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
  avlmap_n *node = avlmap_find (map, key, info);

  if (!node)
    /* not found */
    return;

  avlmap_n *parent = node->parent;

remove:
  if (!node->left && !node->right)
    { /* no children */
      if (!parent)
        {
          free (node);
          map->len--;
          map->root = NULL;
          return;
        }
      if (parent->left == node)
        parent->left = NULL;
      if (parent->right == node)
        parent->right = NULL;

      free (node);
      map->len--;
      goto balance;
    }

  if (!node->left ^ !node->right)
    { /* one children */
      avlmap_n *child = node->left ?: node->right;
      child->parent = parent;
      if (!parent)
        {
          free (node);
          map->len--;
          map->root = child;
          return;
        }
      if (parent->left == node)
        parent->left = child;
      if (parent->right == node)
        parent->right = child;

      free (node);
      map->len--;
      goto balance;
    }

  /* two children */
  avlmap_n *bak = node;
  node = node->right;
  while (node->left)
    node = node->left;

  /* swap data */
  void *bkey = KEY_OF (bak, info);
  void *nkey = KEY_OF (node, info);
  if (!avlmap_swap_data (nkey, bkey, info))
    return;

  parent = node->parent;
  goto remove;

balance:
  for (avlmap_n *curr = parent; curr; curr = curr->parent)
    {
      avlmap_height_t height = parent->height;
      height_update (curr);
      avlmap_n *rotated = rotate (curr);

      if (rotated == curr)
        {
          if (curr->height == height)
            break;
          continue;
        }

      avlmap_n *prnt = rotated->parent;
      if (prnt)
        {
          if (prnt->left == curr)
            prnt->left = rotated;
          else
            prnt->right = rotated;
        }
      else
        {
          map->root = rotated;
          break;
        }

      curr = rotated;
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
  int comp = 0;
  avlmap_n *parent = NULL;

  for (avlmap_n *curr = map->root; curr;)
    {
      void *ckey = KEY_OF (curr, info);
      comp = info->f_comp (key, ckey);

      if (comp == 0)
        return NULL;

      parent = curr;
      curr = comp < 0 ? curr->left : curr->right;
    }

  /* allocate node and initialize */
  avlmap_n *node = malloc (info->n_size);
  if (!node)
    return NULL;

  node->height = 0;
  node->parent = parent;
  node->left = node->right = NULL;

  void *nkey = KEY_OF (node, info);
  void *nval = VAL_OF (node, info);
  if (memcpy (nkey, key, info->k_size) != nkey)
    goto error;
  if (memcpy (nval, val, info->v_size) != nval)
    goto error;

  /* insert node */
  map->len++;
  if (parent)
    {
      if (comp < 0)
        parent->left = node;
      if (comp > 0)
        parent->right = node;
      height_update (parent);
    }
  else
    {
      map->root = node;
      return node;
    }

  /* rebalance */
  for (avlmap_n *curr = parent->parent; curr; curr = curr->parent)
    {
      if (curr->height == height_update (curr))
        break;

      avlmap_n *rotated = rotate (curr);
      if (rotated == curr)
        continue;

      avlmap_n *prnt = rotated->parent;
      if (prnt)
        {
          if (prnt->left == curr)
            prnt->left = rotated;
          else
            prnt->right = rotated;
        }
      else
        {
          map->root = rotated;
          break;
        }

      curr = rotated;
    }

  return node;

error:
  free (node);
  return NULL;
}
