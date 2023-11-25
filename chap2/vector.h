/* vector */

#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include <stdint.h>

/* vector expansion ratio */
#define VECTOR_EXPAN_RATIO 1.5
/* vector initial capacity */
#define VECTOR_INIT_CAP 8

/* vector node */
struct vector_n
{
    union {
        uint8_t u8;
        uint16_t u16;
        uint32_t u32;
        uint64_t u64;

        int8_t i8;
        int16_t i16;
        int32_t i32;
        int64_t i64;

        float f32;
        double f64;

        void *ptr;
        char buf[8];
    };
};

/* vector container (header) */
struct vector_s
{
    size_t size;
    size_t capacity;
    struct vector_n *head;
};

typedef struct vector_s vector;

/*
 * init `vec`.
 */
extern vector *vector_init(vector *vec);

/*
 * preset the space of `cap` elements for `vec`. if `cap` is less than the
 * capacity of `vec`, it will return directly.
 * NULL will be returned if resize the capacity of `vec` failed.
 */
extern vector *vector_reserve(vector *vec, size_t cap);

/*
 * free `vec`.
 */
extern void vector_free(vector *vec);

/*
 * get the pointer of element at `pos` in `vec`.
 * `pos` must be a valid index (0 <= pos < vec->size), otherwise NULL will be
 * returned.
 */
extern struct vector_n *vector_get(vector *vec, size_t pos);

/*
 * insert `node` into `pos` of `vec`.
 * `pos` must be a valid index (0 <= pos < vec->size), otherwise NULL will be
 * returned.
 */
extern struct vector_n *vector_insert(vector *vec, size_t pos,
                                      struct vector_n node);

/*
 * push `node` into the end of `vec`.
 */
extern struct vector_n *vector_push_end(vector *vec, struct vector_n node);

/*
 * remove the element at `pos` in `vec`.
 * `pos` must be a valid index (0 <= pos < vec->size), otherwise NULL will be
 * returned.
 */
extern struct vector_n *vector_remove(vector *vec, size_t pos);

#endif
