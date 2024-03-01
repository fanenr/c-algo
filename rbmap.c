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
rotate_left (rbmap_n *node)
{
  rbmap_n *child = node->right;
  rbmap_n *parent = node->parent;

  if ((node->right = child->left))
    child->left->parent = node;

  if ((child->parent = parent))
    *(parent->left == node ? &parent->left : &parent->right) = child;

  child->left = node;
  node->parent = child;

  return child;
}

static inline rbmap_n *
rotate_right (rbmap_n *node)
{
  rbmap_n *child = node->left;
  rbmap_n *parent = node->parent;

  if ((node->left = child->right))
    child->right->parent = node;

  if ((child->parent = node->parent))
    *(parent->left == node ? &parent->left : &parent->right) = child;

  child->right = node;
  node->parent = child;

  return child;
}

/*
void
rbmap_remove (rbmap *map, void *key, const rbmap_i *info)
{
}
*/

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
      if (!prnt)
        {
          curr->color = RBMAP_BLACK;
          break;
        }
      if (IS_BLACK (prnt))
        break;

      rbmap_n *gprnt = prnt->parent;
      if (!gprnt)
        {
          prnt->color = RBMAP_BLACK;
          break;
        }

      int direction = (prnt == gprnt->left) ? -1 : 1;
      rbmap_n *uncle = direction < 0 ? gprnt->right : gprnt->left;
      if (IS_RED (uncle))
        {
          gprnt->color = RBMAP_RED;
          prnt->color = uncle->color = RBMAP_BLACK;
          curr = gprnt;
          if (curr->parent)
            comp = (curr == curr->parent->left) ? -1 : 1;
          continue;
        }

      rbmap_n *rotated = NULL;
      if (direction < 0 && comp < 0)
        {
          rotated = rotate_right (gprnt);
          gprnt->color = RBMAP_RED;
          prnt->color = RBMAP_BLACK;
        }
      else if (direction > 0 && comp > 0)
        {
          rotated = rotate_left (gprnt);
          gprnt->color = RBMAP_RED;
          prnt->color = RBMAP_BLACK;
        }
      else if (direction < 0)
        {
          rotate_left (prnt);
          rotated = rotate_right (gprnt);
          curr->color = RBMAP_BLACK;
          gprnt->color = RBMAP_RED;
        }
      else if (direction > 0)
        {
          rotate_right (prnt);
          rotated = rotate_left (gprnt);
          curr->color = RBMAP_BLACK;
          gprnt->color = RBMAP_RED;
        }

      if (!rotated->parent)
        map->root = rotated;
      break;
    }

  return node;
}
