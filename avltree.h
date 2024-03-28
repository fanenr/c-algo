#ifndef AVLTREE_H
#define AVLTREE_H

#include "util.h"
#include <stddef.h>

typedef int avltree_height_t;
typedef struct avltree_t avltree_t;
typedef struct avltree_node_t avltree_node_t;

struct avltree_t
{
  size_t size;
  avltree_node_t *root;
};

struct avltree_node_t
{
  avltree_node_t *left;
  avltree_node_t *right;
  avltree_node_t *parent;
  avltree_height_t height;
};

#define AVLTREE_INIT                                                          \
  (avltree_t) {}

extern void avltree_link (avltree_t *tree, avltree_node_t **inpos,
                          avltree_node_t *parent, avltree_node_t *node)
    attr_nonnull (1, 2, 4);

extern void avltree_erase (avltree_t *tree, avltree_node_t *node)
    attr_nonnull (1, 2);

#endif
