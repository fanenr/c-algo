#ifndef VECTOR_H
#define VECTOR_H

#include "util.h"
#include <stddef.h>

#define VECTOR_INIT_CAP 8
#define VECTOR_EXPAN_RATIO 2

typedef struct vector_t vector_t;
typedef void vector_dtor_t (void *e);
typedef int vector_comp_t (const void *a, const void *b);

struct vector_t
{
  void *data;
  size_t cap;
  size_t size;
  size_t elem_size;
  vector_comp_t *elem_comp;
  vector_dtor_t *elem_dtor;
};

extern void vector_init (vector_t *vec, size_t size, vector_comp_t *comp,
                         vector_dtor_t *dtor) attr_nonnull (1);

extern void vector_free (vector_t *vec) attr_nonnull (1);

extern void *vector_at (const vector_t *vec, size_t pos) attr_nonnull (1);

extern void *vector_find (const vector_t *vec, const void *target)
    attr_nonnull (1, 2);

extern vector_t *vector_reserve (vector_t *vec, size_t cap) attr_nonnull (1);

extern void *vector_push_back (vector_t *vec) attr_nonnull (1);

extern void *vector_insert (vector_t *vec, size_t pos) attr_nonnull (1);

extern void vector_remove (vector_t *vec, size_t pos) attr_nonnull (1);

extern void vector_sort (vector_t *vec) attr_nonnull (1);

#endif
