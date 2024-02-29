#include "rbmap.h"
#include <stdbool.h>
#include <stdlib.h>

#define IS_RED(NODE) ((NODE) ? (NODE)->color == RBMAP_RED : false)
#define IS_BLACK(NODE) ((NODE) ? (NODE)->color == RBMAP_BLACK : true)

#define KEY_OF(NODE, INFO) ((void *)(NODE) + (INFO)->k_offs)
#define VAL_OF(NODE, INFO) ((void *)(NODE) + (INFO)->v_offs)

void
rbmap_init (rbmap *map)
{
  *map = (rbmap){ .len = 0, .root = NULL };
}

void
rbmap_free_impl (rbmap_n *node)
{
  if (!node)
    return;

  rbmap_free_impl (node->left);
  rbmap_free_impl (node->right);
  free (node);
}

void
rbmap_free (rbmap *map)
{
  rbmap_free_impl (map->root);
  rbmap_init (map);
}

static inline rbmap_n *
rotate_left (rbmap_n *node)
{
  rbmap_n *child = node->right;

  node->right = child->left;
  child->left = node;

  return child;
}

static inline rbmap_n *
rotate_right (rbmap_n *node)
{
  rbmap_n *child = node->left;

  node->left = child->right;
  child->right = node;

  return child;
}

void
rbmap_remove (rbmap *map, void *key, const rbmap_i *info)
{
}

rbmap_n *
rbmap_find (rbmap *map, void *key, const rbmap_i *info)
{
  if (!map->len)
    return NULL;

  for (rbmap_n *curr = map->root; curr;)
    {
      void *ckey = KEY_OF (curr, info);
      int comp = info->f_comp (key, ckey);

      if (comp == 0)
        return curr;

      curr = comp < 0 ? curr->left : curr->right;
    }

  return NULL;
}

rbmap_n *
rbmap_insert (rbmap *map, void *key, void *val, const rbmap_i *info)
{
}
