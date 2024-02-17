#include "avltree.h"
#include <stdlib.h>
#include <string.h>

#define MAX(A, B) (((A) > (B)) ? (A) : (B))
#define HEIGHT_OF(NODE) ((NODE) ? (NODE)->height : -1)
#define BALANCE_FACTOR_OF(NODE)                                               \
  ((NODE) ? HEIGHT_OF ((NODE)->left) - HEIGHT_OF ((NODE)->right) : 0)

#define KEY_OF(NODE, INFO) ((NODE) + (INFO)->k_offs)
#define VAL_OF(NODE, INFO) ((NODE) + (INFO)->v_offs)

void
avltree_init (avltree *tree)
{
  *tree = (avltree){ .len = 0, .root = NULL };
}

static inline void
avltree_free_impl (avltree_n *node)
{
  if (!node)
    return;
  avltree_free_impl (node->left);
  avltree_free_impl (node->right);
  free (node);
}

void
avltree_free (avltree *tree)
{
  avltree_free_impl (tree->root);
  avltree_init (tree);
}

static inline void
height_update (avltree_n *node)
{
  int lh = HEIGHT_OF (node->left);
  int rh = HEIGHT_OF (node->right);
  node->height = MAX (lh, rh) + 1;
}

static inline avltree_n *
rotate_left (avltree_n *node)
{
  avltree_n *child = node->right;
  avltree_n *gchild = child->left;

  child->left = node;
  node->right = gchild;

  height_update (node);
  height_update (child);
  return child;
}

static inline avltree_n *
rotate_right (avltree_n *node)
{
  avltree_n *child = node->left;
  avltree_n *gchild = child->right;

  child->right = node;
  node->left = gchild;

  height_update (node);
  height_update (child);
  return child;
}

static inline avltree_n *
rotate (avltree_n *node)
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

avltree_n *
avltree_find (avltree *tree, void *key, const avltree_i *info)
{
  if (!tree->len)
    return NULL;

  for (avltree_n *curr = tree->root; curr;)
    {
      void *ckey = KEY_OF (curr, info);
      int comp = info->f_comp (key, ckey);

      if (comp == 0)
        return curr;

      if (comp < 0)
        curr = curr->left;
      else
        curr = curr->right;
    }

  return NULL;
}

static inline avltree_n *
avltree_insert_impl (avltree_n **inpos, avltree_n *curr, void *key, void *val,
                     const avltree_i *info)
{
  if (curr)
    {
      void *ckey = KEY_OF (curr, info);
      int comp = info->f_comp (key, ckey);
      if (comp == 0)
        return curr;

      avltree_n **next = comp < 0 ? &curr->left : &curr->right;
      *next = avltree_insert_impl (inpos, *next, key, val, info);

      height_update (curr);
      curr = rotate (curr);
      return curr;
    }

  void *node = malloc (sizeof (info->n_size));
  void *nkey = KEY_OF (node, info);
  void *nval = VAL_OF (node, info);

  if (!node)
    return NULL;
  if (memcpy (nkey, key, info->k_size) != nkey)
    goto err;
  if (memcpy (nval, val, info->v_size) != nval)
    goto err;

  *inpos = node;
  return node;

err:
  free (node);
  return NULL;
}

avltree_n *
avltree_insert (avltree *tree, void *key, void *val, const avltree_i *info)
{
  avltree_n *sts = NULL;
  tree->root = avltree_insert_impl (&sts, tree->root, key, val, info);
  return sts;
}
