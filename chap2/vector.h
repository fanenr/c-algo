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

extern vector *vector_init(vector *vec);

#endif