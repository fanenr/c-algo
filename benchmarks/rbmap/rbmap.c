#include "rbmap.h"
#include <stdbool.h>
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

static inline rbmap_n *
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

  return child;
}

static inline rbmap_n *
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

  return child;
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

  if (node->left && node->right)
    {
      rbmap_n *old = node;
      node = node->right;
      while (node->left)
        node = node->left;
      if (!rbmap_swap_data (old, node, info))
        return;
    }

  rbmap_n *child = NULL;
  rbmap_n **rmpos = &map->root;
  if (node->parent)
    rmpos = (node == node->parent->left) ? &node->parent->left
                                         : &node->parent->right;

  if (IS_RED (node) && !node->left && !node->right)
    goto del;

  if (!node->left ^ !node->right)
    {
      child = node->left ? node->left : node->right;
      child->parent = node->parent;
      child->color = RBMAP_BLACK;
      goto del;
    }

  for (rbmap_n *curr = node; curr;)
    {
      rbmap_n *prnt = curr->parent;
      int direction = (curr == prnt->left) ? -1 : 1;
      rbmap_n *bro = direction < 0 ? prnt->right : prnt->left;

      if (IS_RED (bro))
        {
          if (direction < 0)
            rotate_left (map, prnt);
          else
            rotate_right (map, prnt);

          prnt->color = RBMAP_RED;
          bro->color = RBMAP_BLACK;
          break;
        }

      if (!bro)
        {
          curr = prnt;
          continue;
        }

      if (IS_RED (bro->left) || IS_RED (bro->right))
        {
          if (direction < 0 && !IS_RED (bro->right))
            {
              rotate_right (map, bro);
              bro = bro->parent;
              bro->color = RBMAP_BLACK;
              bro->right->color = RBMAP_RED;
            }
          else if (direction > 0 && !IS_RED (bro->left))
            {
              rotate_left (map, bro);
              bro = bro->parent;
              bro->color = RBMAP_BLACK;
              bro->left->color = RBMAP_RED;
            }

          if (direction < 0)
            rotate_left (map, prnt);
          else
            rotate_right (map, prnt);

          bro->color = prnt->color;
          prnt->color = RBMAP_BLACK;
          break;
        }

      if (IS_RED (prnt))
        {
          prnt->color = RBMAP_BLACK;
          bro->color = RBMAP_RED;
          break;
        }

      bro->color = RBMAP_RED;
      curr = prnt;
    }

del:
  map->root->color = RBMAP_BLACK;
  *rmpos = child;
  free (node);
  map->size--;
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

  map->size++;
  *inpos = node;

  for (rbmap_n *curr = node; curr;)
    {
      rbmap_n *prnt = curr->parent;
      if (IS_BLACK (prnt) || !prnt->parent)
        break;

      rbmap_n *gprnt = prnt->parent;
      int direction = (prnt == gprnt->left) ? -1 : 1;
      rbmap_n *uncle = direction < 0 ? gprnt->right : gprnt->left;
      if (IS_RED (uncle))
        {
          gprnt->color = RBMAP_RED;
          prnt->color = uncle->color = RBMAP_BLACK;
          curr = gprnt;
          if (!curr->parent)
            break;
          comp = (curr == curr->parent->left) ? -1 : 1;
          continue;
        }

      if (direction < 0 && comp < 0)
        {
          rotate_right (map, gprnt);
          gprnt->color = RBMAP_RED;
          prnt->color = RBMAP_BLACK;
        }
      else if (direction > 0 && comp > 0)
        {
          rotate_left (map, gprnt);
          gprnt->color = RBMAP_RED;
          prnt->color = RBMAP_BLACK;
        }
      else if (direction < 0)
        {
          rotate_left (map, prnt);
          rotate_right (map, gprnt);
          curr->color = RBMAP_BLACK;
          gprnt->color = RBMAP_RED;
        }
      else
        {
          rotate_right (map, prnt);
          rotate_left (map, gprnt);
          curr->color = RBMAP_BLACK;
          gprnt->color = RBMAP_RED;
        }
      break;
    }

  map->root->color = RBMAP_BLACK;
  return node;
}
