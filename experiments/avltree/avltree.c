#include "avltree.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define IS_BALANCED(BF) (-1 <= (BF) && (BF) >= 1)
#define HEIGHT_OF(NODE) ((NODE) ? (NODE)->height : -1)
#define BALANCE_FACTOR_OF(NODE)                                               \
  ((NODE) ? HEIGHT_OF ((NODE)->left) - HEIGHT_OF ((NODE)->right) : 0)

void
avltree_init (avltree_t *tree, avltree_comp_t *comp, avltree_dtor_t *dtor)
{
  *tree = (avltree_t){ .node_comp = comp, .node_dtor = dtor };
}

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

void
avltree_remove (avltree_t *tree, avltree_node_t *node)
{
  avltree_node_t *prnt;
  avltree_node_t **rmpos = &tree->root;
  avltree_dtor_t *node_dtor = tree->node_dtor;

  if ((prnt = node->parent))
    rmpos = (node == prnt->left) ? &prnt->left : &prnt->right;

  if (!node->left && !node->right)
    {
      *rmpos = NULL;
      goto balance;
    }

  if (!node->left ^ !node->right)
    {
      avltree_node_t *child = node->left ?: node->right;
      child->parent = prnt;
      *rmpos = child;
      goto balance;
    }

  avltree_node_t *node2 = prnt = node->right;
  avltree_node_t **rmpos2 = &node2->right;

  for (avltree_node_t *left; (left = node2->left);)
    {
      rmpos2 = &node2->left;
      prnt = node2;
      node2 = left;
    }

  avltree_node_t *next = node2->right;
  *rmpos = node2;
  *node2 = *node;

  *rmpos2 = next;
  next ? next->parent = prnt : 0;

  node->left->parent = node2;
  if (node->right != node2)
    node->right->parent = node2;

balance:
  for (avltree_node_t *curr = prnt; curr; curr = curr->parent)
    {
      if (!curr->parent)
        break;

      avltree_height_t height = curr->height;
      height_update (curr);

      avltree_height_t bf = BALANCE_FACTOR_OF (curr);
      if (IS_BALANCED (bf))
        {
          if (height == curr->height)
            break;
          continue;
        }

      rotate (tree, curr);
      curr = curr->parent;
    }

  if (node_dtor)
    node_dtor (node);
  tree->size--;
}

avltree_node_t *
avltree_find (const avltree_t *tree, const avltree_node_t *target)
{
  if (!tree->size)
    return NULL;

  avltree_comp_t *node_comp = tree->node_comp;

  for (avltree_node_t *curr = tree->root; curr;)
    {
      int comp = node_comp (target, curr);

      if (comp == 0)
        return curr;

      curr = comp < 0 ? curr->left : curr->right;
    }

  return NULL;
}

avltree_node_t *
avltree_insert (avltree_t *tree, avltree_node_t *node)
{
  avltree_node_t **inpos = &tree->root;
  avltree_comp_t *node_comp = tree->node_comp;

  for (avltree_node_t *curr = tree->root; curr;)
    {
      int comp = node_comp (node, curr);

      if (comp == 0)
        return NULL;

      node->parent = curr;
      curr = *(inpos = comp < 0 ? &curr->left : &curr->right);
    }

  *inpos = node;

  for (avltree_node_t *curr = node->parent; curr; curr = curr->parent)
    {
      avltree_height_t height = curr->height;
      height_update (curr);

      if (!curr->parent)
        break;

      if (height == curr->height)
        break;

      avltree_height_t bf = BALANCE_FACTOR_OF (curr);
      if (IS_BALANCED (bf))
        continue;

      rotate (tree, curr);
      curr = curr->parent;
    }

  tree->size++;
  return node;
}

static inline void
avltree_free_impl (avltree_t *tree, avltree_node_t *node)
{
  if (!node)
    return;

  avltree_free_impl (tree, node->left);
  avltree_free_impl (tree, node->right);
  tree->node_dtor (node);
}

void
avltree_free (avltree_t *tree)
{
  avltree_free_impl (tree, tree->root);
  avltree_init (tree, tree->node_comp, tree->node_dtor);
}
