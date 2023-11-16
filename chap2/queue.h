#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>
#include <stdint.h>

/* queue node */
struct queue_n
{
    struct queue_n *next;
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

/* queue container (header) */
struct queue_s
{
    size_t size;
    struct queue_n *head;
    struct queue_n *tail;
};

typedef struct queue_s queue;

/*
 * init `que`.
 */
extern queue *queue_init(queue *que);

/*
 * release `que`.
 */
extern void queue_free(queue *que);

/*
 * allocate a node.
 */
extern struct queue_n *queue_node(void);

/*
 * push `node` into `que`.
 * the `.size` of `que` will be increased after `node` is inserted.
 */
extern struct queue_n *queue_push(queue *restrict que,
                                  struct queue_n *restrict node);

/*
 * get ref of the head node of `que`.
 * NULL will be returned if `que` is empty.
 */
extern struct queue_n *queue_top(queue *que);

/*
 * pop the head node of `que`.
 * no nodes will be removed if `que` is empty.
 * the `.size` of `que` will be decreased after the top node is poped.
 */
extern struct queue_n queue_pop(queue *que);

#endif