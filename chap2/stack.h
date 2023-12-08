/* stack */

#ifndef STACK_H
#define STACK_H

#include <bits/types/stack_t.h>
#include <stddef.h>
#include <stdint.h>

/* stack expansion ratio */
#define STACK_EXPAN_RATIO 1.5
/* stack initial capacity */
#define STACK_INIT_CAP    8

/* stack node */
struct stack_n
{
    union
    {
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

/* stack container (header) */
struct stack_s
{
    size_t size;
    size_t capacity;
    struct stack_n *data;
};

typedef struct stack_s stack;

/*
 * init `stac`.
 */
extern stack *stack_init(stack *stac);

/*
 * preset the space of `cap` elements for `stac`. if `cap` is less than the
 * capacity of `stac`, it will return directly.
 * NULL will be returned if resize the capacity of `stac` failed.
 */
extern stack *stack_reserve(stack *stac, size_t cap);

/*
 * release `stac`.
 */
extern void stack_free(stack *stac);

/*
 * push `node` into `stac`.
 * if the capacity of `stac` is not enough to accommodate one node, the
 * capacity of `stac` will be expanded to STACK_EXPAN_RATIO times (the
 * `.capacity` of `stac` will be changed too).
 * if there is enough space, `node` will be pushed into `stac` and the `.size`
 * of `stac` will be increased.
 */
extern struct stack_n *stack_push(stack *stac, struct stack_n node);

/*
 * get ref of the top node of `stac`.
 * NULL will be returned if `stac` is empty.
 */
extern struct stack_n *stack_top(stack *stac);

/*
 * pop the top node of `stac`.
 * no nodes will be removed if `stac` is empty.
 * the `.size` of `stac` will be decreased after the top node is poped.
 */
extern struct stack_n stack_pop(stack *stac);

static inline stack
stack_new(void)
{
    stack stac;
    stack_init(&stac);
    return stac;
}

static inline size_t
stack_size(const stack *stac)
{
    return stac == NULL ? 0 : stac->size;
}

static inline size_t
stack_cap(const stack *stac)
{
    return stac == NULL ? 0 : stac->capacity;
}

static inline const struct stack_n *
stack_data(const stack *stac)
{
    return stac == NULL ? 0 : stac->data;
}

#endif
