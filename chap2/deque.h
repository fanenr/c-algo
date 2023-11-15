#ifndef DEQUE_H
#define DEQUE_H

#include <stddef.h>
#include <stdint.h>

/* deque node */
struct deque_n
{
    struct deque_n *prev;
    struct deque_n *next;
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
    } data;
};

/* deque container (header) */
struct deque_s
{
    size_t size;
    struct deque_n *head;
};

typedef struct deque_s deque;

/*
 * init `que`.
 */
extern deque *deque_init(deque *que);

/*
 * release `que`.
 */
extern void deque_free(deque *que);

/*
 * allocate a node.
 */
extern struct deque_n *deque_node(void);

/*
 * push `node` to the end of `que`.
 * the `.size` of `que` will be increased after `node` is pushed.
 */
extern struct deque_n *deque_push_end(deque *restrict que,
                                      struct deque_n *restrict node);

/*
 * push `node` to the head of `que`.
 * the `.size` of `que` will be increased after `node` is pushed.
 */
extern struct deque_n *deque_push_head(deque *restrict que,
                                       struct deque_n *restrict node);

/*
 * get ref of the head node of `que`.
 * NULL will be returned if `que` is empty.
 */
extern struct deque_n *deque_top(deque *que);

/*
 * pop the head node of `que`.
 * no nodes will be removed if `que` is empty.
 * the `.size` of `que` will be decreased after the top node is poped.
 */
extern struct deque_n deque_pop(deque *que);

#endif