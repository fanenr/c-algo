#include "avltree.h"

#define HEIGHT_OF(NODE) ((NODE) ? (NODE)->height : -1)

#define BALANCE_FACTOR_OF(NODE)                                               \
  ((NODE) ? HEIGHT_OF ((NODE)->left) - HEIGHT_OF ((NODE)->right) : 0)

static inline avltree_height_t
height_update (avltree_node_t *node)
{
  avltree_height_t lh = HEIGHT_OF (node->left);
  avltree_height_t rh = HEIGHT_OF (node->right);
  return (node->height = (lh > rh ? lh : rh) + 1);
}

static inline avltree_node_t *
rotate_left (avltree_t *tree, avltree_node_t *node)
{
  avltree_node_t *child = node->right;
  avltree_node_t *parent = node->parent;

  if ((node->right = child->left))
    node->right->parent = node;

  if (parent)
    if (node == parent->left)
      parent->left = child;
    else
      parent->right = child;
  else
    tree->root = child;
  child->parent = parent;

  child->left = node;
  node->parent = child;

  height_update (node);
  height_update (child);

  return child;
}

static inline avltree_node_t *
rotate_right (avltree_t *tree, avltree_node_t *node)
{
  avltree_node_t *child = node->left;
  avltree_node_t *parent = node->parent;

  if ((node->left = child->right))
    node->left->parent = node;

  if (parent)
    if (node == parent->left)
      parent->left = child;
    else
      parent->right = child;
  else
    tree->root = child;
  child->parent = parent;

  child->right = node;
  node->parent = child;

  height_update (node);
  height_update (child);

  return child;
}

static inline avltree_node_t *
rotate (avltree_t *tree, avltree_node_t *node)
{
  avltree_height_t bf = BALANCE_FACTOR_OF (node);

  if (bf > 1)
    {
      if (BALANCE_FACTOR_OF (node->left) >= 0)
        return rotate_right (tree, node);

      rotate_left (tree, node->left);
      return rotate_right (tree, node);
    }

  if (bf < -1)
    {
      if (BALANCE_FACTOR_OF (node->right) <= 0)
        return rotate_left (tree, node);

      rotate_right (tree, node->right);
      return rotate_left (tree, node);
    }

  return node;
}

extern void
avltree_link (avltree_t *tree, avltree_node_t **inpos, avltree_node_t *parent,
              avltree_node_t *node)
{
  node->left = node->right = NULL;
  node->parent = parent;
  node->height = 0;
  *inpos = node;

  for (avltree_node_t *curr = parent; curr; curr = curr->parent)
    {
      avltree_height_t height = curr->height;
      height_update (curr);

      if (height == curr->height)
        break;

      if (curr == rotate (tree, curr))
        continue;

      curr = curr->parent;
    }

  tree->size++;
}

void
avltree_erase (avltree_t *tree, avltree_node_t *node)
{
  avltree_node_t **rmpos;
  avltree_node_t *left = node->left;
  avltree_node_t *right = node->right;
  avltree_node_t *parent = node->parent;
  avltree_node_t *child = left ? left : right;

  rmpos = parent ? (node == parent->left) ? &parent->left : &parent->right
                 : &tree->root;

  if (left && right)
    {
      avltree_node_t *next = right;
      for (avltree_node_t *temp; (temp = next->left);)
        next = temp;

      child = next->right;
      parent = next->parent;

      left->parent = next;
      right->parent = next;

      *rmpos = next;
      *next = *node;

      if (next == right)
        {
          parent = next;
          rmpos = &next->right;
        }
      else
        rmpos = &parent->left;
    }

  *rmpos = child;

  if (child)
    child->parent = parent;

  for (avltree_node_t *curr = parent; curr; curr = curr->parent)
    {
      avltree_height_t height = curr->height;
      height_update (curr);

      if (curr == rotate (tree, curr))
        {
          if (height == curr->height)
            break;
          continue;
        }

      curr = curr->parent;
    }

  tree->size--;
}

avltree_node_t *
avltree_find (const avltree_t *tree, const avltree_node_t *target,
              avltree_comp_t *comp)
{
  for (avltree_node_t *curr = tree->root; curr;)
    {
      int comp_ret = comp (target, curr);

      if (comp_ret == 0)
        return curr;

      curr = comp_ret < 0 ? curr->left : curr->right;
    }

  return NULL;
}

avltree_node_t *
avltree_insert (avltree_t *tree, avltree_node_t *node, avltree_comp_t *comp)
{
  avltree_node_t *parent = NULL;
  avltree_node_t **inpos = &tree->root;

  for (avltree_node_t *curr = tree->root; curr;)
    {
      int comp_ret = comp (node, curr);

      if (comp_ret != 0)
        {
          parent = curr;
          inpos = comp_ret < 0 ? &curr->left : &curr->right;
          curr = *inpos;
          continue;
        }

      return NULL;
    }

  avltree_link (tree, inpos, parent, node);

  return node;
}

static inline void
avltree_free_impl (avltree_node_t *node, avltree_dtor_t *dtor)
{
  if (!node)
    return;

  avltree_free_impl (node->left, dtor);
  avltree_free_impl (node->right, dtor);
  dtor (node);
}

void
avltree_free (avltree_t *tree, avltree_dtor_t *dtor)
{
  avltree_free_impl (tree->root, dtor);
  *tree = AVLTREE_INIT;
}
