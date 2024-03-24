#ifndef AVLTREE_H
#define AVLTREE_H

#include "util.h"
#include <stddef.h>

typedef int avltree_height_t;
typedef struct avltree_t avltree_t;
typedef struct avltree_node_t avltree_node_t;

typedef int avltree_comp_t (const avltree_node_t *a, const avltree_node_t *b);

struct avltree_t
{
  size_t size;
  avltree_node_t *root;
  avltree_comp_t *comp_fn;
};

struct avltree_node_t
{
  avltree_node_t *left;
  avltree_node_t *right;
  avltree_height_t height;
};

#define avltree_node_init                                                     \
  (avltree_node_t) {}

extern void avltree_init (avltree_t *tree, avltree_comp_t *comp);

extern avltree_node_t *avltree_insert (avltree_t *tree, avltree_node_t *node)
    attr_nonnull (1, 2);

extern avltree_node_t *avltree_remove (avltree_t *tree,
                                       const avltree_node_t *target)
    attr_nonnull (1, 2);

extern avltree_node_t *avltree_find (const avltree_t *tree,
                                     const avltree_node_t *target)
    attr_nonnull (1, 2);

#endif
