#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

#define VECTOR_INIT_CAP 8
#define VECTOR_EXPAN_RATIO 2

typedef struct vector vector;
typedef struct vector_i vector_i;

struct vector_i
{
  size_t d_size;
};

struct vector
{
  size_t cap;
  size_t size;
  void *data;
};

extern void vector_init (vector *vec) __attribute__ ((nonnull (1)));

extern void vector_free (vector *vec) __attribute__ ((nonnull (1)));

extern void vector_remove (vector *vec, size_t pos, const vector_i info)
    __attribute__ ((nonnull (1)));

extern vector *vector_reserve (vector *vec, size_t cap, const vector_i info)
    __attribute__ ((nonnull (1)));

extern void *vector_at (const vector *vec, size_t pos, const vector_i info)
    __attribute__ ((nonnull (1)));

extern void *vector_push_back (vector *vec, void *data, const vector_i info)
    __attribute__ ((nonnull (1, 2)));

extern void *vector_insert (vector *vec, size_t pos, void *data,
                            const vector_i info)
    __attribute__ ((nonnull (1, 3)));

#define VECTOR_DEF_INIT(PRE, DTYPE)                                           \
  static inline void PRE##_vector_init (PRE##_vector *vec)                    \
  {                                                                           \
    vector_init ((vector *)vec);                                              \
  }

#define VECTOR_DEF_FREE(PRE, DTYPE)                                           \
  static inline void PRE##_vector_free (PRE##_vector *vec)                    \
  {                                                                           \
    vector_free ((vector *)vec);                                              \
  }

#define VECTOR_DEF_REMOVE(PRE, DTYPE)                                         \
  static inline void PRE##_vector_remove (PRE##_vector *vec, size_t pos)      \
  {                                                                           \
    vector_remove ((vector *)vec, pos, PRE##_vector_info);                    \
  }

#define VECTOR_DEF_RESERVE(PRE, DTYPE)                                        \
  static inline PRE##_vector *PRE##_vector_reserve (PRE##_vector *vec,        \
                                                    size_t cap)               \
  {                                                                           \
    return (PRE##_vector *)vector_reserve ((vector *)vec, cap,                \
                                           PRE##_vector_info);                \
  }

#define VECTOR_DEF_AT(PRE, DTYPE)                                             \
  static inline DTYPE *PRE##_vector_at (const PRE##_vector *vec, size_t pos)  \
  {                                                                           \
    return vector_at ((vector *)vec, pos, PRE##_vector_info);                 \
  }

#define VECTOR_DEF_PUSH_BACK(PRE, DTYPE)                                      \
  static inline DTYPE *PRE##_vector_push_back (PRE##_vector *vec, DTYPE data) \
  {                                                                           \
    return vector_push_back ((vector *)vec, &data, PRE##_vector_info);        \
  }

#define VECTOR_DEF_INSERT(PRE, DTYPE)                                         \
  static inline DTYPE *PRE##_vector_insert (PRE##_vector *vec, size_t pos,    \
                                            DTYPE data)                       \
  {                                                                           \
    return vector_insert ((vector *)vec, pos, &data, PRE##_vector_info);      \
  }

#define VECTOR_DEF_INFO(PRE, DTYPE)                                           \
  typedef struct PRE##_vector PRE##_vector;                                   \
                                                                              \
  struct PRE##_vector                                                         \
  {                                                                           \
    size_t cap;                                                               \
    size_t size;                                                              \
    DTYPE *data;                                                              \
  };                                                                          \
                                                                              \
  static const vector_i PRE##_vector_info = { .d_size = sizeof (DTYPE) };     \
                                                                              \
  static void PRE##_vector_init (PRE##_vector *vec)                           \
      __attribute__ ((nonnull (1)));                                          \
                                                                              \
  static void PRE##_vector_free (PRE##_vector *vec)                           \
      __attribute__ ((nonnull (1)));                                          \
                                                                              \
  static void PRE##_vector_remove (PRE##_vector *vec, size_t pos)             \
      __attribute__ ((nonnull (1)));                                          \
                                                                              \
  static PRE##_vector *PRE##_vector_reserve (PRE##_vector *vec, size_t cap)   \
      __attribute__ ((nonnull (1)));                                          \
                                                                              \
  static DTYPE *PRE##_vector_at (const PRE##_vector *vec, size_t pos)         \
      __attribute__ ((nonnull (1)));                                          \
                                                                              \
  static DTYPE *PRE##_vector_push_back (PRE##_vector *vec, DTYPE data)        \
      __attribute__ ((nonnull (1)));                                          \
                                                                              \
  static DTYPE *PRE##_vector_insert (PRE##_vector *vec, size_t pos,           \
                                     DTYPE data)                              \
      __attribute__ ((nonnull (1)));

#define VECTOR_DEF_ALL(PRE, DTYPE)                                            \
  VECTOR_DEF_INFO (PRE, DTYPE)                                                \
                                                                              \
  VECTOR_DEF_AT (PRE, DTYPE)                                                  \
  VECTOR_DEF_INIT (PRE, DTYPE)                                                \
  VECTOR_DEF_FREE (PRE, DTYPE)                                                \
  VECTOR_DEF_INSERT (PRE, DTYPE)                                              \
  VECTOR_DEF_REMOVE (PRE, DTYPE)                                              \
  VECTOR_DEF_RESERVE (PRE, DTYPE)                                             \
  VECTOR_DEF_PUSH_BACK (PRE, DTYPE)

#endif
