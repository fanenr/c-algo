#ifndef AVLTREE_H
#define AVLTREE_H

#include "util.h"
#include <stddef.h>

typedef int avltree_height_t;
typedef struct avltree_t avltree_t;
typedef struct avltree_node_t avltree_node_t;

typedef void avltree_dtor_t (avltree_node_t *n);
typedef int avltree_comp_t (const avltree_node_t *a, const avltree_node_t *b);

struct avltree_t
{
  size_t size;
  avltree_node_t *root;
  avltree_comp_t *node_comp;
  avltree_dtor_t *node_dtor;
};

struct avltree_node_t
{
  avltree_node_t *left;
  avltree_node_t *right;
  avltree_node_t *parent;
  avltree_height_t height;
};

#define avltree_node_init                                                     \
  (avltree_node_t) {}

extern void avltree_init (avltree_t *tree, avltree_comp_t *comp,
                          avltree_dtor_t *dtor) attr_nonnull (1, 2);

extern avltree_node_t *avltree_find (const avltree_t *tree,
                                     const avltree_node_t *target)
    attr_nonnull (1, 2);

extern avltree_node_t *avltree_insert (avltree_t *tree, avltree_node_t *node)
    attr_nonnull (1, 2);

extern void avltree_remove (avltree_t *tree, avltree_node_t *node)
    attr_nonnull (1, 2);

extern void avltree_free (avltree_t *tree) attr_nonnull (1);

#endif
