#ifndef AVLTREE_EXT_H
#define AVLTREE_EXT_H

#include "avltree.h"

typedef void avltree_visit_t (avltree_node_t *n);
typedef int avltree_comp_t (const avltree_node_t *a, const avltree_node_t *b);

extern avltree_node_t *avltree_find (const avltree_t *tree,
                                     const avltree_node_t *target,
                                     avltree_comp_t *comp)
    attr_nonnull (1, 2, 3);

extern avltree_node_t *avltree_insert (avltree_t *tree, avltree_node_t *node,
                                       avltree_comp_t *comp)
    attr_nonnull (1, 2, 3);

extern void avltree_for_each (avltree_t *tree, avltree_visit_t *visit)
    attr_nonnull (1, 2);

#endif
