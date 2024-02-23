#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

#define VECTOR_INIT_CAP 8
#define VECTOR_EXPAN_RATIO 2

typedef struct vector_i
{
  size_t d_size;
} vector_i;

typedef struct vector
{
  size_t cap;
  size_t len;
  void *data;
} vector;

extern void vector_init (vector *vec) __attribute__ ((nonnull (1)));

extern void vector_free (vector *vec) __attribute__ ((nonnull (1)));

extern void vector_remove (vector *vec, size_t pos, const vector_i *info)
    __attribute__ ((nonnull (1, 3)));

extern vector *vector_reserve (vector *vec, size_t cap, const vector_i *info)
    __attribute__ ((nonnull (1, 3)));

extern void *vector_at (const vector *vec, size_t pos, const vector_i *info)
    __attribute__ ((nonnull (1, 3)));

extern void *vector_push_back (vector *vec, void *data, const vector_i *info)
    __attribute__ ((nonnull (1, 2, 3)));

extern void *vector_insert (vector *vec, size_t pos, void *data,
                            const vector_i *info)
    __attribute__ ((nonnull (1, 3, 4)));

#define VECTOR_DEF_REMOVE(TYPE, PRE)                                          \
  static inline void PRE##_vector_remove (vector *vec, size_t pos)            \
  {                                                                           \
    vector_remove (vec, pos, &PRE##_vector_info);                             \
  }

#define VECTOR_DEF_RESERVE(TYPE, PRE)                                         \
  static inline vector *PRE##_vector_reserve (vector *vec, size_t cap)        \
  {                                                                           \
    return vector_reserve (vec, cap, &PRE##_vector_info);                     \
  }

#define VECTOR_DEF_AT(TYPE, PRE)                                              \
  static inline TYPE *PRE##_vector_at (const vector *vec, size_t pos)         \
  {                                                                           \
    return vector_at (vec, pos, &PRE##_vector_info);                          \
  }

#define VECTOR_DEF_PUSH_BACK(TYPE, PRE)                                       \
  static inline TYPE *PRE##_vector_push_back (vector *vec, TYPE data)         \
  {                                                                           \
    return vector_push_back (vec, &data, &PRE##_vector_info);                 \
  }

#define VECTOR_DEF_INSERT(TYPE, PRE)                                          \
  static inline TYPE *PRE##_vector_insert (vector *vec, size_t pos,           \
                                           TYPE data)                         \
  {                                                                           \
    return vector_insert (vec, pos, &data, &PRE##_vector_info);               \
  }

#define VECTOR_DEF_INFO(TYPE, PRE)                                            \
  static const vector_i PRE##_vector_info = { .d_size = sizeof (TYPE) };      \
                                                                              \
  static void PRE##_vector_remove (vector *vec, size_t pos)                   \
      __attribute__ ((nonnull (1)));                                          \
                                                                              \
  static vector *PRE##_vector_reserve (vector *vec, size_t cap)               \
      __attribute__ ((nonnull (1)));                                          \
                                                                              \
  static TYPE *PRE##_vector_at (const vector *vec, size_t pos)                \
      __attribute__ ((nonnull (1)));                                          \
                                                                              \
  static inline TYPE *PRE##_vector_push_back (vector *vec, TYPE data)         \
      __attribute__ ((nonnull (1)));                                          \
                                                                              \
  static TYPE *PRE##_vector_insert (vector *vec, size_t pos, TYPE data)       \
      __attribute__ ((nonnull (1)));

#define VECTOR_DEF_ALL(TYPE, PRE)                                             \
  VECTOR_DEF_INFO (TYPE, PRE);                                                \
  VECTOR_DEF_REMOVE (TYPE, PRE);                                              \
  VECTOR_DEF_RESERVE (TYPE, PRE);                                             \
  VECTOR_DEF_AT (TYPE, PRE);                                                  \
  VECTOR_DEF_PUSH_BACK (TYPE, PRE);                                           \
  VECTOR_DEF_INSERT (TYPE, PRE)

#endif