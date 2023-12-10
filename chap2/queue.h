/* queue */

#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>
#include <stdint.h>

struct queue_v
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

/* queue node */
struct queue_n
{
    struct queue_v data;
    struct queue_n *next;
};

/* queue container (header) */
typedef struct
{
    size_t size;
    struct queue_n *head;
} queue;

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
 * push a node into `que`.
 */
extern struct queue_n *queue_push(queue *que, struct queue_v data);

/*
 * pop a node from `que`.
 */
extern struct queue_n queue_pop(queue *que);

static inline queue
queue_new(void)
{
    queue que;
    queue_init(&que);
    return que;
}

static inline size_t
queue_size(const queue *que)
{
    return que == NULL ? 0 : que->size;
}

static inline struct queue_n *
queue_head(const queue *que)
{
    return que == NULL ? NULL : que->head;
}

#endif
