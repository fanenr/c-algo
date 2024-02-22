#include "avlmap.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define HEIGHT_OF(NODE) ((NODE) ? (NODE)->height : -1)
#define BALANCE_FACTOR_OF(NODE)                                               \
  ((NODE) ? HEIGHT_OF ((NODE)->left) - HEIGHT_OF ((NODE)->right) : 0)

#define KEY_OF(NODE, INFO) ((void *)(NODE) + (INFO)->k_offs)
#define VAL_OF(NODE, INFO) ((void *)(NODE) + (INFO)->v_offs)

void
avlmap_init (avlmap *tree)
{
  *tree = (avlmap){ .len = 0, .root = NULL };
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
avlmap_free (avlmap *tree)
{
  avlmap_free_impl (tree->root);
  avlmap_init (tree);
}

static inline void
height_update (avlmap_n *node)
{
  int lh = HEIGHT_OF (node->left);
  int rh = HEIGHT_OF (node->right);
  node->height = (lh > rh ? lh : rh) + 1;
}

static inline avlmap_n *
rotate_left (avlmap_n *node)
{
  avlmap_n *child = node->right;
  avlmap_n *gchild = child->left;

  child->left = node;
  node->right = gchild;

  height_update (node);
  height_update (child);
  return child;
}

static inline avlmap_n *
rotate_right (avlmap_n *node)
{
  avlmap_n *child = node->left;
  avlmap_n *gchild = child->right;

  child->right = node;
  node->left = gchild;

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

static inline avlmap_n *
avlmap_remove_impl (bool *sts, avlmap_n *curr, void *key, const avlmap_i *info)
{
  if (!curr)
    return NULL;

  void *ckey = KEY_OF (curr, info);
  void *cval = VAL_OF (curr, info);
  int comp = info->f_comp (key, ckey);

  if (comp != 0)
    { /* continue to find */
      avlmap_n **next = comp < 0 ? &curr->left : &curr->right;
      *next = avlmap_remove_impl (sts, *next, key, info);
      goto update;
    }

  if (!curr->left && !curr->right)
    { /* no children */
      free (curr);
      *sts = true;
      return NULL;
    }

  if (!curr->left ^ !curr->right)
    { /* one children */
      avlmap_n *child = curr->left ?: curr->right;
      free (curr);
      curr = child;
      *sts = true;
      goto update;
    }

  /* two children */
  avlmap_n *repl = curr->right;
  while (repl->left)
    repl = repl->left;

  void *rkey = KEY_OF (repl, info);
  void *rval = VAL_OF (repl, info);
  if (memcpy (ckey, rkey, info->k_size) != ckey)
    return curr;
  if (memcpy (cval, rval, info->v_size) != cval)
    /* TODO: need rollback */
    return curr;

  curr->right = avlmap_remove_impl (sts, curr->right, rkey, info);

update:
  height_update (curr);
  curr = rotate (curr);
  return curr;
}

void
avlmap_remove (avlmap *tree, void *key, const avlmap_i *info)
{
  bool sts = false;
  tree->root = avlmap_remove_impl (&sts, tree->root, key, info);
  if (sts)
    tree->len--;
}

avlmap_n *
avlmap_find (avlmap *tree, void *key, const avlmap_i *info)
{
  if (!tree->len)
    return NULL;

  for (avlmap_n *curr = tree->root; curr;)
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
avlmap_insert_impl (avlmap_n **sts, avlmap_n *curr, void *key, void *val,
                    const avlmap_i *info)
{
  if (curr)
    {
      void *ckey = KEY_OF (curr, info);
      int comp = info->f_comp (key, ckey);
      if (comp == 0)
        return curr;

      avlmap_n **next = comp < 0 ? &curr->left : &curr->right;
      *next = avlmap_insert_impl (sts, *next, key, val, info);

      height_update (curr);
      curr = rotate (curr);
      return curr;
    }

  avlmap_n *node = malloc (info->n_size);
  void *nkey = KEY_OF (node, info);
  void *nval = VAL_OF (node, info);

  if (!node)
    return NULL;

  node->height = 0;
  node->left = node->right = NULL;
  if (memcpy (nkey, key, info->k_size) != nkey)
    goto error;
  if (memcpy (nval, val, info->v_size) != nval)
    goto error;

  *sts = node;
  return node;

error:
  free (node);
  return NULL;
}

avlmap_n *
avlmap_insert (avlmap *tree, void *key, void *val, const avlmap_i *info)
{
  avlmap_n *sts = NULL;
  tree->root = avlmap_insert_impl (&sts, tree->root, key, val, info);
  if (sts)
    tree->len++;
  return sts;
}
