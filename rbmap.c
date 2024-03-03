#include "rbmap.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IS_RED(NODE) ((NODE) ? (NODE)->color == RBMAP_RED : false)
#define IS_BLACK(NODE) ((NODE) ? (NODE)->color == RBMAP_BLACK : true)

#define KEY_OF(NODE, INFO) ((void *)(NODE) + (INFO)->k_offs)
#define VAL_OF(NODE, INFO) ((void *)(NODE) + (INFO)->v_offs)

void
rbmap_init (rbmap *map)
{
  *map = (rbmap){ .size = 0, .root = NULL };
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

static inline void
rotate_left (rbmap *map, rbmap_n *node)
{
  rbmap_n *child = node->right;
  rbmap_n **repos = &map->root;
  rbmap_n *parent = node->parent;

  if ((node->right = child->left))
    child->left->parent = node;

  if ((child->parent = parent))
    repos = (node == parent->left) ? &parent->left : &parent->right;
  *repos = child;

  child->left = node;
  node->parent = child;
}

static inline void
rotate_right (rbmap *map, rbmap_n *node)
{
  rbmap_n *child = node->left;
  rbmap_n **repos = &map->root;
  rbmap_n *parent = node->parent;

  if ((node->left = child->right))
    child->right->parent = node;

  if ((child->parent = parent))
    repos = (node == parent->left) ? &parent->left : &parent->right;
  *repos = child;

  child->right = node;
  node->parent = child;
}

static inline bool
rbmap_swap_data (rbmap_n *node1, rbmap_n *node2, const rbmap_i *info)
{
  void *ktemp = NULL;
  void *kpos1 = KEY_OF (node1, info);
  void *kpos2 = KEY_OF (node2, info);
  size_t d_size = info->n_size - info->k_offs;

  if (!(ktemp = alloca (d_size)))
    return false;
  if (memcpy (ktemp, kpos1, d_size) != ktemp)
    return false;
  if (memcpy (kpos1, kpos2, d_size) != kpos1)
    return false;
  if (memcpy (kpos2, ktemp, d_size) != kpos2)
    return false;

  return true;
}

void
rbmap_remove (rbmap *map, void *key, const rbmap_i *info)
{
  rbmap_n *node = rbmap_find (map, key, info);
  if (!node)
    return;

  rbmap_n *prnt = NULL;
  rbmap_n **rmpos = &map->root;
  rbmap_n *child = node->left ? node->left : node->right;

  if (node->left && node->right)
    {
      rbmap_n *old = node, *left;
      node = node->right;
      while ((left = node->left))
        node = left;
      child = node->right;
      if (!rbmap_swap_data (old, node, info))
        return;
    }

  if ((prnt = node->parent))
    rmpos = (node == prnt->left) ? &prnt->left : &prnt->right;

  if (IS_RED (node))
    {
      free (node);
      *rmpos = NULL;
      goto ret;
    }

  if (child)
    {
      free (node);
      *rmpos = child;
      child->parent = prnt;
      child->color = RBMAP_BLACK;
      goto ret;
    }

  for (rbmap_n *curr = node; curr;)
    {
      rbmap_n *prnt = curr->parent;
      if (!prnt)
        break;

      bool curr_left = (curr == prnt->left);
      rbmap_n *bro = curr_left ? prnt->right : prnt->left;

      if (IS_RED (bro))
        {
          prnt->color = RBMAP_RED;
          bro->color = RBMAP_BLACK;
          curr_left ? rotate_left (map, prnt) : rotate_right (map, prnt);
          bro = curr_left ? prnt->right : prnt->left;
        }

      if (IS_BLACK (bro->left) && IS_BLACK (bro->right))
        {
          bro->color = RBMAP_RED;
          if (IS_RED (prnt))
            {
              prnt->color = RBMAP_BLACK;
              break;
            }
          curr = prnt;
          continue;
        }

      if (curr_left)
        {
          if (IS_BLACK (bro->right))
            {
              rbmap_n *b_left;
              if ((b_left = bro->left))
                b_left->color = RBMAP_BLACK;
              bro->color = RBMAP_RED;
              rotate_right (map, bro);
              bro = prnt->right;
            }
          rotate_left (map, prnt);
          if (bro->right)
            bro->right->color = RBMAP_BLACK;
        }
      else
        {
          if (IS_BLACK (bro->left))
            {
              rbmap_n *b_right;
              if ((b_right = bro->right))
                b_right->color = RBMAP_BLACK;
              bro->color = RBMAP_RED;
              rotate_left (map, bro);
              bro = prnt->left;
            }
          rotate_right (map, prnt);
          if (bro->left)
            bro->left->color = RBMAP_BLACK;
        }

      bro->color = prnt->color;
      prnt->color = RBMAP_BLACK;
      break;
    }

  free (node);
  *rmpos = NULL;

ret:
  map->size--;
  if (map->root)
    map->root->color = RBMAP_BLACK;
}

rbmap_n *
rbmap_find (const rbmap *map, void *key, const rbmap_i *info)
{
  if (!map->size)
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

static inline rbmap_n *
rbmap_node_new (void *key, void *val, const rbmap_i *info)
{
  rbmap_n *node = NULL;
  if (!(node = malloc (info->n_size)))
    return NULL;

  node->color = RBMAP_RED;
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

rbmap_n *
rbmap_insert (rbmap *map, void *key, void *val, const rbmap_i *info)
{
  int comp = 0;
  rbmap_n *node = NULL;
  rbmap_n **inpos = &map->root;

  if (!(node = rbmap_node_new (key, val, info)))
    return NULL;

  for (rbmap_n *curr = map->root; curr;)
    {
      void *ckey = KEY_OF (curr, info);
      comp = info->f_comp (key, ckey);

      if (comp == 0)
        return NULL;

      node->parent = curr;
      curr = *(inpos = comp < 0 ? &curr->left : &curr->right);
    }

  *inpos = node;

  for (rbmap_n *curr = node; curr;)
    {
      rbmap_n *prnt = curr->parent;
      if (IS_BLACK (prnt))
        break;

      rbmap_n *gprnt = prnt->parent;
      bool curr_left = (curr == prnt->left);
      bool prnt_left = (prnt == gprnt->left);
      rbmap_n *uncle = prnt_left ? gprnt->right : gprnt->left;

      if (IS_RED (uncle))
        {
          gprnt->color = RBMAP_RED;
          prnt->color = uncle->color = RBMAP_BLACK;
          curr = gprnt;
          continue;
        }

      if (prnt_left && !curr_left)
        {
          rbmap_n *temp = prnt;
          rotate_left (map, prnt);
          prnt = curr;
          curr = temp;
        }
      if (!prnt_left && curr_left)
        {
          rbmap_n *temp = prnt;
          rotate_right (map, prnt);
          prnt = curr;
          curr = temp;
        }

      gprnt->color = RBMAP_RED;
      prnt->color = RBMAP_BLACK;
      if (prnt_left)
        rotate_right (map, gprnt);
      else
        rotate_left (map, gprnt);
      break;
    }

  map->root->color = RBMAP_BLACK;
  map->size++;
  return node;
}
