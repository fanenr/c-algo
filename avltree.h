#ifndef AVLTREE_H
#define AVLTREE_H

#include <stddef.h>

typedef int avltree_comp_t (void *key1, void *key2);

typedef struct avltree_i
{
  size_t k_size;
  size_t v_size;
  size_t n_size;
  size_t k_offs;
  size_t v_offs;
  avltree_comp_t *f_comp;
} avltree_i;

typedef struct avltree_n avltree_n;

struct avltree_n
{
  int height;
  avltree_n *left;
  avltree_n *right;
};

typedef struct avltree
{
  size_t len;
  avltree_n *root;
} avltree;

extern void avltree_init (avltree *tree);

extern void avltree_free (avltree *tree);

extern void avltree_remove (avltree *tree, void *key, const avltree_i *info)
    __attribute__ ((nonnull (1, 2, 3)));

extern avltree_n *avltree_find (avltree *tree, void *key,
                                const avltree_i *info)
    __attribute__ ((nonnull (1, 2, 3)));

extern avltree_n *avltree_insert (avltree *tree, void *key, void *val,
                                  const avltree_i *info)
    __attribute__ ((nonnull (1, 2, 3)));

#endif
