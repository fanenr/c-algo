#ifndef SLIST_H
#define SLIST_H

#include <stddef.h>

/*
 * slist node
 */
struct slist_n
{
    void *value;
    struct slist_n *next;
};

/*
 * slist container (header)
 */
struct slist_s
{
    size_t size;
    struct slist_n *head;
};

typedef struct slist_s slist;

/*
 * init the `list`
 */
extern slist *slist_init(slist *list);

/*
 * free the `list`
 */
extern void slist_free(slist *list);

/*
 * malloc a node
 */
extern struct slist_n *slist_node(void);

/*
 * insert a `node` after `pos` in the `list`
 * if the `pos` is NULL, the `node` will be inserted at the beginning of the
 * `list` insert will change `.size` of the `list` if `node` has been inserted
 */
extern struct slist_n *slist_insert(slist *restrict list,
                                    struct slist_n *restrict pos,
                                    struct slist_n *restrict node);

/*
 * remove a node at `pos` in the `list`
 * no node will be removed if the `pos` is NULL
 * remove will change `.size` of the `list if `pos` has been removed
 */
extern void slist_remove(slist *restrict list, struct slist_n *restrict pos);

#endif