#ifndef DLIST_H
#define DLIST_H

#include <stddef.h>

/*
 * dlist node
 */
struct dlist_n
{
    void *value;
    struct dlist_n *prev;
    struct dlist_n *next;
};

/*
 * dlist container (header)
 */
struct dlist_s
{
    size_t size;
    struct dlist_n *head;
};

typedef struct dlist_s dlist;

/*
 * init the `list`
 */
extern dlist *dlist_init(dlist *list);

/*
 * free the `list`
 */
extern void dlist_free(dlist *list);

/*
 * malloc a node
 */
extern struct dlist_n *dlist_node(void);

/*
 * insert a `node` after `pos` in the `list`
 * if the `pos` is NULL, the `node` will be inserted at the beginning of the
 * `list` insert will change `.size` of the `list` if `node` has been inserted
 */
extern struct dlist_n *dlist_insert(dlist *restrict list,
                                    struct dlist_n *restrict pos,
                                    struct dlist_n *restrict node);

/*
 * remove a node at `pos` in the `list`
 * no node will be removed if the `pos` is NULL
 * remove will change `.size` of the `list if `pos` has been removed
 */
extern void dlist_remove(dlist *restrict list, struct dlist_n *restrict pos);

#endif