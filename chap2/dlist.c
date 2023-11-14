#include "dlist.h"
#include <stdlib.h>
#include <string.h>

dlist *dlist_init(dlist *list)
{
    list->size = 0;
    list->head = NULL;
    return list;
}

void dlist_free(dlist *list)
{
    struct dlist_n *node, *next;
    for (node = list->head; node != NULL; node = next) {
        next = node->next;
        free(node);
    }
    dlist_init(list);
    return;
}

struct dlist_n *dlist_node(void)
{
    struct dlist_n *node = malloc(sizeof(struct dlist_n));
    node->data.ptr = node->prev = node->next = NULL;
    return node;
}

struct dlist_n *dlist_insert(dlist *restrict list, struct dlist_n *restrict pos,
                             struct dlist_n *restrict node)
{
    if (pos == NULL) {
        node->prev = NULL;
        node->next = list->head;
        if (list->head != NULL)
            list->head->prev = node;
        list->head = node;
    } else {
        node->prev = pos;
        node->next = pos->next;
        if (pos->next != NULL)
            pos->next->prev = node;
        pos->next = node;
    }

    list->size += 1;
    return node;
}

void dlist_remove(dlist *restrict list, struct dlist_n *restrict pos)
{
    if (pos == NULL)
        return;

    if (pos == list->head) {
        list->head = pos->next;
        if (pos->next != NULL)
            pos->next->prev = NULL;
        free(pos);
        goto end;
    }

    pos->prev->next = pos->next;
    if (pos->next != NULL)
        pos->next->prev = pos->prev;
    free(pos);

end:
    list->size -= 1;
    return;
}
