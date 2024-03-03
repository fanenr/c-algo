#include "avlmap.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define HEIGHT_MAX 36

#define IS_BALANCED(BF) (-1 <= (BF) && (BF) >= 1)
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

static inline void
height_update (avlmap_n *node)
{
  avlmap_height_t lh = HEIGHT_OF (node->left);
  avlmap_height_t rh = HEIGHT_OF (node->right);
  node->height = (lh > rh ? lh : rh) + 1;
}

static inline void
rotate_left (avlmap *map, avlmap_n *node)
{
  avlmap_n *child = node->right;
  avlmap_n **repos = &map->root;
  avlmap_n *parent = node->parent;

  if ((node->right = child->left))
    node->right->parent = node;

  if ((child->parent = parent))
    repos = (node == parent->left) ? &parent->left : &parent->right;
  *repos = child;

  child->left = node;
  node->parent = child;

  height_update (node);
  height_update (child);
}

static inline void
rotate_right (avlmap *map, avlmap_n *node)
{
  avlmap_n *child = node->left;
  avlmap_n **repos = &map->root;
  avlmap_n *parent = node->parent;

  if ((node->left = child->right))
    node->left->parent = node;

  if ((child->parent = parent))
    repos = (node == parent->left) ? &parent->left : &parent->right;
  *repos = child;

  child->right = node;
  node->parent = child;

  height_update (node);
  height_update (child);
}

static inline void
rotate (avlmap *map, avlmap_n *node)
{
  int bf = BALANCE_FACTOR_OF (node);

  if (bf > 1)
    {
      if (BALANCE_FACTOR_OF (node->left) >= 0)
        return rotate_right (map, node);

      rotate_left (map, node->left);
      rotate_right (map, node);
      return;
    }

  if (bf < -1)
    {
      if (BALANCE_FACTOR_OF (node->right) <= 0)
        return rotate_left (map, node);

      rotate_right (map, node->right);
      rotate_left (map, node);
      return;
    }
}

static inline bool
avlmap_swap_data (avlmap_n *node1, avlmap_n *node2, const avlmap_i *info)
{
  void *bakup = NULL;
  void *kpos1 = KEY_OF (node1, info);
  void *kpos2 = KEY_OF (node2, info);
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
    return;

  avlmap_n *prnt;
  avlmap_n **repos = &map->root;
  avlmap_n *child = node->left ? node->left : node->right;

  if (node->left && node->right)
    {
      avlmap_n *old = node, *left;
      node = node->right;
      while ((left = node->left))
        node = node->left;
      child = node->right;
      if (!avlmap_swap_data (old, node, info))
        return;
    }

  if ((prnt = node->parent))
    repos = (node == prnt->left) ? &prnt->left : &prnt->right;

  free (node);
  *repos = child;
  child ? child->parent = prnt : 0;

  for (avlmap_n *curr = prnt; curr; curr = curr->parent)
    {
      if (!curr->parent)
        break;

      avlmap_height_t height = curr->height;
      height_update (curr);

      avlmap_height_t bf = BALANCE_FACTOR_OF (curr);
      if (IS_BALANCED (bf))
        {
          if (height == curr->height)
            break;
          continue;
        }

      rotate (map, curr);
      curr = curr->parent;
    }

  map->len--;
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

static inline avlmap_n *
avlmap_node_new (void *key, void *val, const avlmap_i *info)
{
  avlmap_n *node = NULL;
  if (!(node = malloc (info->n_size)))
    return NULL;

  node->height = 0;
  node->left = node->right = node->parent = NULL;

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
  avlmap_n *node;
  avlmap_n **inpos = &map->root;

  if (!(node = avlmap_node_new (key, val, info)))
    return NULL;

  for (avlmap_n *curr = map->root; curr;)
    {
      void *ckey = KEY_OF (curr, info);
      int comp = info->f_comp (key, ckey);

      if (comp == 0)
        return NULL;

      node->parent = curr;
      curr = *(inpos = comp < 0 ? &curr->left : &curr->right);
    }

  *inpos = node;

  for (avlmap_n *curr = node->parent; curr; curr = curr->parent)
    {
      avlmap_height_t height = curr->height;
      height_update (curr);

      if (!curr->parent)
        break;

      if (height == curr->height)
        break;

      avlmap_height_t bf = BALANCE_FACTOR_OF (curr);
      if (IS_BALANCED (bf))
        continue;

      rotate (map, curr);
      curr = curr->parent;
    }

  map->len++;
  return node;
}
