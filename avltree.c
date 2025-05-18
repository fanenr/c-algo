#include "avltree.h"

#define likely(exp) __builtin_expect (!!(exp), 1)
#define unlikely(exp) __builtin_expect (!!(exp), 0)

#define H_OF(NODE) ((NODE) ? (NODE)->height : -1)
#define BF_OF(NODE) ((NODE) ? H_OF ((NODE)->left) - H_OF ((NODE)->right) : 0)

static inline avltree_height_t
height_update (avltree_node_t *node)
{
  avltree_height_t lh = H_OF (node->left);
  avltree_height_t rh = H_OF (node->right);
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
  avltree_height_t bf = BF_OF (node);

  if (bf > 1)
    {
      if (BF_OF (node->left) >= 0)
	return rotate_right (tree, node);

      rotate_left (tree, node->left);
      return rotate_right (tree, node);
    }

  if (bf < -1)
    {
      if (BF_OF (node->right) <= 0)
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
avltree_last (const avltree_t *tree)
{
  avltree_node_t *last;

  if ((last = tree->root))
    for (avltree_node_t *right; (right = last->right);)
      last = right;

  return last;
}

avltree_node_t *
avltree_first (const avltree_t *tree)
{
  avltree_node_t *first;

  if ((first = tree->root))
    for (avltree_node_t *left; (left = first->left);)
      first = left;

  return first;
}

avltree_node_t *
avltree_next (const avltree_node_t *node)
{
  avltree_node_t *next;

  if ((next = node->right))
    for (avltree_node_t *left; (left = next->left);)
      next = left;
  else
    for (; (next = node->parent) && node != next->left;)
      node = next;

  return next;
}

avltree_node_t *
avltree_prev (const avltree_node_t *node)
{
  avltree_node_t *prev;

  if ((prev = node->left))
    for (avltree_node_t *right; (right = prev->right);)
      prev = right;
  else
    for (; (prev = node->parent) && node != prev->right;)
      node = prev;

  return prev;
}

/* **************************************************************** */
/*                               ext                                */
/* **************************************************************** */

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
  int comp_ret = 0;
  avltree_node_t **inpos;
  avltree_node_t *parent = NULL;

  for (avltree_node_t *curr = tree->root; curr;)
    {
      comp_ret = comp (node, curr);

      if (unlikely (comp_ret == 0))
	return NULL;

      parent = curr;
      curr = comp_ret < 0 ? curr->left : curr->right;
    }

  inpos = comp_ret ? (comp_ret < 0 ? &parent->left : &parent->right)
		   : &tree->root;

  avltree_link (tree, inpos, parent, node);

  return node;
}

static inline void
avltree_visit_impl (avltree_node_t *node, avltree_visit_t *func)
{
  if (node)
    {
      avltree_node_t *left = node->left;
      avltree_node_t *right = node->right;

      avltree_visit_impl (left, func);
      func (node);
      avltree_visit_impl (right, func);
    }
}

void
avltree_visit (avltree_t *tree, avltree_visit_t *func)
{
  avltree_visit_impl (tree->root, func);
}
