/* double-linked list */

#ifndef DLIST_H
#define DLIST_H

#include <stddef.h>
#include <stdint.h>

struct dlist_v
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

/* dlist node */
struct dlist_n
{
    struct dlist_v data;
    struct dlist_n *prev;
    struct dlist_n *next;
};

/* dlist container (header) */
typedef struct
{
    size_t size;
    struct dlist_n *head;
    struct dlist_n *tail;
} dlist;

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
 * find the first node whose data equals to `data` from `spos` (include `spos`).
 * a pointer of that found node will be returned if found, otherwise NULL will
 * be returend.
 */
extern struct dlist_n *dlist_find_from(dlist *restrict list,
                                       struct dlist_n *restrict spos,
                                       struct dlist_v data);

/*
 * find the first node whose data equals to `data` in `list`.
 */
extern struct dlist_n *dlist_find(dlist *list, struct dlist_v data);

/*
 * insert a node before `pos` into `list`.
 * a pointer of inserted node will be returned if insert successfully, otherwise
 * NULL will be returned.
 */
extern struct dlist_n *dlist_insert(dlist *restrict list,
                                    struct dlist_n *restrict pos,
                                    struct dlist_v data);

/*
 * insert a node into the end of `list`.
 * a pointer of inserted node will be returned if insert successfully, otherwise
 * NULL will be returned.
 */
extern struct dlist_n *dlist_push_back(dlist *list, struct dlist_v data);

/*
 * insert a node into the front of `list`.
 * a pointer of inserted node will be returned if insert successfully, otherwise
 * NULL will be returned.
 */
extern struct dlist_n *dlist_push_front(dlist *list, struct dlist_v data);

/*
 * remove the node at `pos` in `list`.
 * `pos` will be returned if erased successfully, otherwise NULL will be
 * returned.
 */
extern struct dlist_n *dlist_erase(dlist *restrict list,
                                   struct dlist_n *restrict pos);

/*
 * remove the first node whose data equals to `data` from `spos`
 * (include `spos`).
 */
extern void dlist_remove_from(dlist *restrict list,
                              struct dlist_n *restrict spos,
                              struct dlist_v data);
/*
 * remove the first node whose data equals to `data` in `list`.
 */
extern void dlist_remove(dlist *list, struct dlist_v data);

/*
 * remove the last node of `list`.
 */
extern void dlist_pop_back(dlist *list);

/*
 * remove the front node of `list`.
 */
extern void dlist_pop_front(dlist *list);

static inline dlist
dlist_new(void)
{
    dlist list;
    dlist_init(&list);
    return list;
}

static inline size_t
dlist_size(dlist *list)
{
    return list == NULL ? 0 : list->size;
}

static inline struct dlist_n *
dlist_head(dlist *list)
{
    return list == NULL ? NULL : list->head;
}

static inline struct dlist_n *
dlist_tail(dlist *list)
{
    return list == NULL ? NULL : list->tail;
}

#endif
