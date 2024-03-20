#ifndef VECTOR_H
#define VECTOR_H

#include "util.h"
#include <stddef.h>

#define VECTOR_INIT_CAP 8
#define VECTOR_EXPAN_RATIO 2

typedef struct vector_t vector_t;

struct vector_t
{
  size_t cap;
  void *data;
  size_t size;
  size_t elem_size;
};

extern vector_t *vector_reserve (vector_t *vec, size_t cap) attr_nonnull (1);

extern void vector_init (vector_t *vec, size_t elem_size) attr_nonnull (1);

extern void *vector_at (const vector_t *vec, size_t pos) attr_nonnull (1);

extern void *vector_insert (vector_t *vec, size_t pos) attr_nonnull (1);

extern void vector_remove (vector_t *vec, size_t pos) attr_nonnull (1);

extern void *vector_push_back (vector_t *vec) attr_nonnull (1);

extern void vector_free (vector_t *vec) attr_nonnull (1);

#endif
