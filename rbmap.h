#ifndef RBMAP_H
#define RBMAP_H

#include <stddef.h>

typedef int rbmap_comp_t (const void *key1, const void *key2);

typedef struct rbmap_i
{
  size_t k_size;
  size_t v_size;
  size_t n_size;
  size_t k_offs;
  size_t v_offs;
  rbmap_comp_t *f_comp;
} rbmap_i;

#define RBMAP_RED 0
#define RBMAP_BLACK 1

typedef int rbmap_color_t;
typedef struct rbmap_n rbmap_n;

struct rbmap_n
{
  rbmap_n *left;
  rbmap_n *right;
  rbmap_color_t color;
};

typedef struct rbmap
{
  size_t len;
  rbmap_n *root;
} rbmap;

extern void rbmap_init (rbmap *map);

extern void rbmap_free (rbmap *map);

extern void rbmap_remove (rbmap *map, void *key, const rbmap_i *info);

extern rbmap_n *rbmap_find (rbmap *map, void *key, const rbmap_i *info);

extern rbmap_n *rbmap_insert (rbmap *map, void *key, void *val,
                              const rbmap_i *info);

#endif
