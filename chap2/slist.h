/* single-linked list */

#ifndef SLIST_H
#define SLIST_H

#include <stddef.h>
#include <stdint.h>

/* slist node */
struct slist_n
{
    struct slist_n *next;
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

/* slist container (header) */
struct slist_s
{
    size_t size;
    struct slist_n *head;
};

typedef struct slist_s slist;

/*
 * init `list`.
 */
extern slist *slist_init(slist *list);

/*
 * release `list`.
 */
extern void slist_free(slist *list);

/*
 * allocate a node.
 */
extern struct slist_n *slist_node(void);

/*
 * insert `node` after `pos` in `list`.
 * if `pos` is NULL, `node` will be inserted at the beginning of `list`. the
 * `.size` of `list` will be increased after `node` is inserted.
 */
extern struct slist_n *slist_insert(slist *restrict list,
                                    struct slist_n *restrict pos,
                                    struct slist_n *restrict node);

/*
 * remove the node at `pos` in `list`.
 * no nodes will be removed if the `pos` is NULL. the `.size` of `list` will be
 * decreased after the node at `pos` is removed.
 */
extern void slist_remove(slist *restrict list, struct slist_n *restrict pos);

#endif
