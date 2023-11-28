/* double-linked list */

#ifndef DLIST_H
#define DLIST_H

#include <stddef.h>
#include <stdint.h>

/* dlist node */
struct dlist_n
{
    struct dlist_n *prev;
    struct dlist_n *next;
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
    } data;
};

/* dlist container (header) */
struct dlist_s
{
    size_t size;
    struct dlist_n *head;
};

typedef struct dlist_s dlist;

/*
 * init `list`.
 */
extern dlist *dlist_init(dlist *list);

/*
 * release `list`.
 */
extern void dlist_free(dlist *list);

/*
 * allocate a node.
 */
extern struct dlist_n *dlist_node(void);

/*
 * insert `node` after `pos` in `list`.
 * if `pos` is NULL, `node` will be inserted at the beginning of `list`. the
 * `.size` of `list` will be increased after `node` is inserted.
 */
extern struct dlist_n *dlist_insert(dlist *restrict list,
                                    struct dlist_n *restrict pos,
                                    struct dlist_n *restrict node);

/*
 * remove the node at `pos` in `list`.
 * no nodes will be removed if the `pos` is NULL. the `.size` of `list` will be
 * decreased after the node at `pos` is removed.
 */
extern void dlist_remove(dlist *restrict list, struct dlist_n *restrict pos);

#endif
