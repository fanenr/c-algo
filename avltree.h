#ifndef AVLTREE_H
#define AVLTREE_H

#include <stddef.h>

#define container_of(ptr, type, member)                                       \
  ((type *)((void *)(ptr) - offsetof (type, member)))

#define attr_nonnull(...) __attribute__ ((nonnull (__VA_ARGS__)))

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

extern avltree_node_t *avltree_last (const avltree_t *tree) attr_nonnull (1);

extern avltree_node_t *avltree_first (const avltree_t *tree) attr_nonnull (1);

extern avltree_node_t *avltree_next (const avltree_node_t *node)
    attr_nonnull (1);

extern avltree_node_t *avltree_prev (const avltree_node_t *node)
    attr_nonnull (1);

/* **************************************************************** */
/*                               ext                                */
/* **************************************************************** */

typedef void avltree_visit_t (avltree_node_t *n);

typedef int avltree_comp_t (const avltree_node_t *a, const avltree_node_t *b);

extern avltree_node_t *avltree_find (const avltree_t *tree,
                                     const avltree_node_t *target,
                                     avltree_comp_t *comp)
    attr_nonnull (1, 2, 3);

extern avltree_node_t *avltree_insert (avltree_t *tree, avltree_node_t *node,
                                       avltree_comp_t *comp)
    attr_nonnull (1, 2, 3);

extern void avltree_visit (avltree_t *tree, avltree_visit_t *func)
    attr_nonnull (1, 2);

#endif
