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
 * empty a slist
 */
extern slist *slist_empty(slist *list);

/*
 * free a list
 */
extern void slist_free(slist *list);

/*
 * insert
 */
extern void slist_insert(slist *list, struct slist_n *pos,
                         struct slist_n *node);

/*
 * remove
 */
extern void slist_remove(slist *list, struct slist_n *pos);

#endif