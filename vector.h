#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

#define VECTOR_INIT_CAP 8
#define VECTOR_EXPAN_RATIO 2

typedef struct vector
{
  size_t cap;
  size_t len;
  void *data;
} vector;

extern void vector_init (vector *vec) __attribute__ ((nonnull (1)));

extern void vector_free (vector *vec) __attribute__ ((nonnull (1)));

extern vector *vector_reserve (vector *vec, size_t cap, size_t ele)
    __attribute__ ((nonnull (1)));

extern void *vector_at (vector *vec, size_t pos, size_t ele)
    __attribute__ ((nonnull (1)));

extern void *vector_remove (vector *vec, size_t pos, size_t ele)
    __attribute__ ((nonnull (1)));

extern void *vector_push_back (vector *restrict vec, void *restrict data,
                               size_t ele) __attribute__ ((nonnull (1, 2)));

extern void *vector_insert (vector *restrict vec, size_t pos,
                            void *restrict data, size_t ele)
    __attribute__ ((nonnull (1, 3)));

#define VECTOR_DEFS(TYPE, PRE)                                                \
  __attribute__ ((nonnull (1))) static inline vector *PRE##_vector_reserve (  \
      vector *vec, size_t cap)                                                \
  {                                                                           \
    return vector_reserve (vec, cap, sizeof (TYPE));                          \
  }                                                                           \
                                                                              \
  __attribute__ ((nonnull (1))) static inline TYPE *PRE##_vector_at (         \
      vector *vec, size_t pos)                                                \
  {                                                                           \
    return vector_at (vec, pos, sizeof (TYPE));                               \
  }                                                                           \
                                                                              \
  __attribute__ ((nonnull (1))) static inline TYPE *PRE##_vector_remove (     \
      vector *vec, size_t pos)                                                \
  {                                                                           \
    return vector_remove (vec, pos, sizeof (TYPE));                           \
  }                                                                           \
                                                                              \
  __attribute__ ((nonnull (1))) static inline TYPE *PRE##_vector_push_back (  \
      vector *vec, TYPE data)                                                 \
  {                                                                           \
    return vector_push_back (vec, &data, sizeof (TYPE));                      \
  }                                                                           \
                                                                              \
  __attribute__ ((nonnull (1))) static inline TYPE *PRE##_vector_insert (     \
      vector *vec, size_t pos, TYPE data)                                     \
  {                                                                           \
    return vector_insert (vec, pos, &data, sizeof (TYPE));                    \
  }

#endif