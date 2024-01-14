/* deque */

#ifndef DEQUE_H
#define DEQUE_H

#include <stddef.h>
#include <stdint.h>

struct deque_v
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

/* deque node */
struct deque_n
{
  struct deque_v data;
  struct deque_n *prev;
  struct deque_n *next;
};

/* deque container (header) */

typedef struct
{
  size_t size;
  struct deque_n *head;
  struct deque_n *tail;
} deque;

/*
 * init `que`.
 */
extern deque *deque_init (deque *que);

/*
 * release `que`.
 */
extern void deque_free (deque *que);

/*
 * allocate a node.
 */
extern struct deque_n *deque_node (void);

/*
 * insert a node before `pos` into `que`.
 * a pointer of inserted node will be returned if insert successfully,
 * otherwise NULL will be returned.
 */
extern struct deque_n *deque_insert (deque *restrict que,
                                     struct deque_n *restrict pos,
                                     struct deque_v data);

/*
 * insert a node into the end of `que`.
 */
extern struct deque_n *deque_push_back (deque *que, struct deque_v data);

/*
 * insert a node into the front of `que`.
 */
extern struct deque_n *deque_push_front (deque *que, struct deque_v data);

/*
 * remove the node at `pos` in `que`.
 */
extern struct deque_n *deque_erase (deque *restrict que,
                                    struct deque_n *restrict pos);

/*
 * remove the last node of `que`.
 */
extern void deque_pop_back (deque *que);

/*
 * remove the front node of `que`.
 */
extern void deque_pop_front (deque *que);

static inline deque
deque_new (void)
{
  deque que;
  deque_init (&que);
  return que;
}

static inline size_t
deque_size (const deque *que)
{
  return que == NULL ? 0 : que->size;
}

static inline struct deque_n *
deque_head (const deque *que)
{
  return que == NULL ? NULL : que->head;
}

static inline struct deque_n *
deque_tail (const deque *que)
{
  return que == NULL ? NULL : que->tail;
}

#endif
