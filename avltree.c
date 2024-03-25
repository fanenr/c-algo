#include "avltree.h"

#define BALANCE_CHECK(BF) (-2 < (BF) && (BF) < 2)

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

static inline void
rotate_left (avltree_t *tree, avltree_node_t *node)
{
  avltree_node_t *child = node->right;
  avltree_node_t **repos = &tree->root;
  avltree_node_t *parent = node->parent;

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
rotate_right (avltree_t *tree, avltree_node_t *node)
{
  avltree_node_t *child = node->left;
  avltree_node_t **repos = &tree->root;
  avltree_node_t *parent = node->parent;

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
rotate (avltree_t *tree, avltree_node_t *node)
{
  avltree_height_t bf = BALANCE_FACTOR_OF (node);

  if (bf > 1)
    {
      if (BALANCE_FACTOR_OF (node->left) >= 0)
        return rotate_right (tree, node);

      rotate_left (tree, node->left);
      rotate_right (tree, node);
      return;
    }

  if (bf < -1)
    {
      if (BALANCE_FACTOR_OF (node->right) <= 0)
        return rotate_left (tree, node);

      rotate_right (tree, node->right);
      rotate_left (tree, node);
      return;
    }
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

      avltree_height_t bf = BALANCE_FACTOR_OF (curr);
      if (BALANCE_CHECK (bf))
        continue;

      rotate (tree, curr);
      curr = curr->parent;
    }

  tree->size++;
}

void
avltree_erase (avltree_t *tree, avltree_node_t *node)
{
  avltree_node_t *parent;
  avltree_node_t *left = node->left;
  avltree_node_t *right = node->right;
  avltree_node_t **rmpos = &tree->root;

  if ((parent = node->parent))
    rmpos = (node == parent->left) ? &parent->left : &parent->right;

  if (!left && !right)
    {
      *rmpos = NULL;
      goto balance;
    }

  if (!left ^ !right)
    {
      avltree_node_t *child = left ? left : right;
      child->parent = parent;
      *rmpos = child;
      goto balance;
    }

  avltree_node_t *next = parent = node->right;
  avltree_node_t **rmpos2 = &next->right;

  for (avltree_node_t *left; (left = next->left);)
    {
      rmpos2 = &next->left;
      parent = next;
      next = left;
    }

  avltree_node_t *child = next->right;
  *next = *node;
  *rmpos = next;

  child ? child->parent = parent : 0;
  *rmpos2 = child;

  if (next != right)
    node->right->parent = next;
  node->left->parent = next;

balance:
  for (avltree_node_t *curr = parent; curr; curr = curr->parent)
    {
      avltree_height_t height = curr->height;
      height_update (curr);

      avltree_height_t bf = BALANCE_FACTOR_OF (curr);

      if (BALANCE_CHECK (bf))
        {
          if (height == curr->height)
            break;
          continue;
        }

      rotate (tree, curr);
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

      if (comp_ret == 0)
        return NULL;

      parent = curr;
      inpos = comp_ret < 0 ? &curr->left : &curr->right;
      curr = *inpos;
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
