#include "slist.h"
#include <stdlib.h>

slist *slist_init(slist *list)
{
    list->size = 0;
    list->head = NULL;
    return list;
}

void slist_free(slist *list)
{
    struct slist_n *node, *next;
    for (node = list->head; node != NULL; node = next) {
        next = node->next;
        free(node);
    }
    slist_init(list);
    return;
}

struct slist_n *slist_node(void)
{
    struct slist_n *node = malloc(sizeof(struct slist_n));
    node->data.ptr = node->next = NULL;
    return node;
}

struct slist_n *slist_insert(slist *restrict list, struct slist_n *restrict pos,
                             struct slist_n *restrict node)
{
    if (pos == NULL) {
        node->next = list->head;
        list->head = node;
    } else {
        node->next = pos->next;
        pos->next = node;
    }

    list->size += 1;
    return node;
}

void slist_remove(slist *restrict list, struct slist_n *restrict pos)
{
    if (pos == NULL)
        return;

    struct slist_n *rm = list->head;
    if (pos == rm) {
        list->head = rm->next;
        free(rm);
        goto end;
    }

    struct slist_n *prev = list->head;
    while (pos != prev->next)
        prev = prev->next;

    rm = prev->next;
    prev->next = rm->next;
    free(rm);

end:
    list->size -= 1;
    return;
}
