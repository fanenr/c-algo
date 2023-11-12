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
    struct slist_n *node = list->head;
    while (node != NULL) {
        free(node);
        node = node->next;
    }
    slist_init(list);
}