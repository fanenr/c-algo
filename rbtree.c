#include "rbtree.h"
#include <stdbool.h>

#define IS_RED(NODE) ((NODE) ? (NODE)->color == RBTREE_RED : false)
#define IS_BLACK(NODE) ((NODE) ? (NODE)->color == RBTREE_BLACK : true)

static inline void
rotate_left (rbtree_t *tree, rbtree_node_t *node)
{
  rbtree_node_t *child = node->right;
  rbtree_node_t **repos = &tree->root;
  rbtree_node_t *parent = node->parent;

  if ((node->right = child->left))
    child->left->parent = node;

  if ((child->parent = parent))
    repos = (node == parent->left) ? &parent->left : &parent->right;
  *repos = child;

  child->left = node;
  node->parent = child;
}

static inline void
rotate_right (rbtree_t *tree, rbtree_node_t *node)
{
  rbtree_node_t *child = node->left;
  rbtree_node_t **repos = &tree->root;
  rbtree_node_t *parent = node->parent;

  if ((node->left = child->right))
    child->right->parent = node;

  if ((child->parent = parent))
    repos = (node == parent->left) ? &parent->left : &parent->right;
  *repos = child;

  child->right = node;
  node->parent = child;
}

void
rbtree_link (rbtree_t *tree, rbtree_node_t **inpos, rbtree_node_t *parent,
             rbtree_node_t *node)
{
  node->left = node->right = NULL;
  node->color = RBTREE_RED;
  node->parent = parent;
  *inpos = node;

  for (rbtree_node_t *curr = node; curr;)
    {
      rbtree_node_t *prnt = curr->parent;
      if (IS_BLACK (prnt))
        break;

      rbtree_node_t *gprnt = prnt->parent;
      bool curr_left = (curr == prnt->left);
      bool prnt_left = (prnt == gprnt->left);
      rbtree_node_t *uncle = prnt_left ? gprnt->right : gprnt->left;

      if (IS_RED (uncle))
        {
          gprnt->color = RBTREE_RED;
          prnt->color = uncle->color = RBTREE_BLACK;
          curr = gprnt;
          continue;
        }

      if (prnt_left)
        {
          if (!curr_left)
            rotate_left (tree, prnt);
          rotate_right (tree, gprnt);
        }
      else
        {
          if (curr_left)
            rotate_right (tree, prnt);
          rotate_left (tree, gprnt);
        }

      gprnt->parent->color = RBTREE_BLACK;
      gprnt->color = RBTREE_RED;
      break;
    }

  tree->size++;
  tree->root->color = RBTREE_BLACK;
}

void
rbtree_erase (rbtree_t *tree, rbtree_node_t *node)
{
  rbtree_node_t *parent;
  rbtree_node_t *left = node->left;
  rbtree_node_t *right = node->right;
  rbtree_node_t **rmpos = &tree->root;
  rbtree_node_t *child = left ? left : right;

  if ((parent = node->parent))
    rmpos = (node == parent->left) ? &parent->left : &parent->right;

  if (left && right)
    {
      rbtree_node_t *next = right;

      for (rbtree_node_t *temp; (temp = next->left);)
        next = temp;

      rbtree_node_t old = *node;
      rbtree_node_t *next_parent = next->parent;

      if (left)
        left->parent = next;

      if (right)
        right->parent = next;

      if (next->right)
        next->right->parent = node;

      *rmpos = next;
      *node = *next;
      *next = old;

      if (next == right)
        {
          rmpos = &next->right;
          next->right = node;
          parent = next;
        }
      else
        {
          rmpos = &next_parent->left;
          next_parent->left = node;
          parent = next_parent;
        }

      child = node->right;
    }

  if (node->color == RBTREE_RED)
    {
      *rmpos = NULL;
      goto ret;
    }

  if (child)
    {
      child->color = RBTREE_BLACK;
      child->parent = parent;
      *rmpos = child;
      goto ret;
    }

  for (rbtree_node_t *curr = node; curr;)
    {
      rbtree_node_t *prnt = curr->parent;
      if (!prnt)
        break;

      bool curr_left = (curr == prnt->left);
      rbtree_node_t *bro = curr_left ? prnt->right : prnt->left;

      if (IS_RED (bro))
        {
          prnt->color = RBTREE_RED;
          bro->color = RBTREE_BLACK;
          curr_left ? rotate_left (tree, prnt) : rotate_right (tree, prnt);
          bro = curr_left ? prnt->right : prnt->left;
        }

      if (IS_BLACK (bro->left) && IS_BLACK (bro->right))
        {
          bro->color = RBTREE_RED;
          if (IS_RED (prnt))
            {
              prnt->color = RBTREE_BLACK;
              break;
            }
          curr = prnt;
          continue;
        }

      if (curr_left)
        {
          if (IS_BLACK (bro->right))
            {
              bro->left->color = RBTREE_BLACK;
              bro->color = RBTREE_RED;
              rotate_right (tree, bro);
              bro = prnt->right;
            }
          rotate_left (tree, prnt);
          bro->right->color = RBTREE_BLACK;
        }
      else
        {
          if (IS_BLACK (bro->left))
            {
              bro->right->color = RBTREE_BLACK;
              bro->color = RBTREE_RED;
              rotate_left (tree, bro);
              bro = prnt->left;
            }
          rotate_right (tree, prnt);
          bro->left->color = RBTREE_BLACK;
        }

      bro->color = prnt->color;
      prnt->color = RBTREE_BLACK;
      break;
    }

  *rmpos = NULL;

ret:
  tree->size--;
  if (tree->root)
    tree->root->color = RBTREE_BLACK;
}

rbtree_node_t *
rbtree_insert (rbtree_t *tree, rbtree_node_t *node, rbtree_comp_t *comp)
{
  rbtree_node_t *parent = NULL;
  rbtree_node_t **inpos = &tree->root;

  for (rbtree_node_t *curr = tree->root; curr;)
    {
      int comp_ret = comp (node, curr);

      if (comp_ret == 0)
        return NULL;

      parent = curr;
      inpos = comp_ret < 0 ? &curr->left : &curr->right;
      curr = *inpos;
    }

  rbtree_link (tree, inpos, parent, node);

  return node;
}

rbtree_node_t *
rbtree_find (const rbtree_t *tree, const rbtree_node_t *target,
             rbtree_comp_t *comp)
{
  for (rbtree_node_t *curr = tree->root; curr;)
    {
      int comp_ret = comp (target, curr);

      if (comp_ret == 0)
        return curr;

      curr = comp_ret < 0 ? curr->left : curr->right;
    }

  return NULL;
}

static inline void
rbtree_free_impl (rbtree_node_t *node, rbtree_dtor_t *dtor)
{
  if (!node)
    return;

  rbtree_free_impl (node->left, dtor);
  rbtree_free_impl (node->right, dtor);
  dtor (node);
}

void
rbtree_free (rbtree_t *tree, rbtree_dtor_t *dtor)
{
  rbtree_free_impl (tree->root, dtor);
  *tree = RBTREE_INIT;
}
