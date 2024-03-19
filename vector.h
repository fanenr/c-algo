#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

#define VECTOR_INIT_CAP 8
#define VECTOR_EXPAN_RATIO 2

typedef struct vector_t vector_t;
typedef int vector_comp_t (const void *a, const void *b);

struct vector_t
{
  size_t cap;
  void *data;
  size_t size;
  size_t elem_size;
  vector_comp_t *comp_func;
};

#define vector_nonnull(...) __attribute__ ((nonnull (__VA_ARGS__)))

extern void vector_init (vector_t *vec, size_t elem_size,
                         vector_comp_t *comp_func) vector_nonnull (1);

extern vector_t *vector_reserve (vector_t *vec, size_t cap) vector_nonnull (1);

extern void *vector_at (const vector_t *vec, size_t pos) vector_nonnull (1);

extern void *vector_insert (vector_t *vec, size_t pos) vector_nonnull (1);

extern void vector_remove (vector_t *vec, size_t pos) vector_nonnull (1);

extern void *vector_push_back (vector_t *vec) vector_nonnull (1);

extern void vector_free (vector_t *vec) vector_nonnull (1);

#undef vector_nonnull

#endif
