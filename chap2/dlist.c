#include "dlist.h"
#include <stdlib.h>

dlist *
dlist_init(dlist *list)
{
    if (list == NULL)
        return NULL;

    list->size = 0;
    list->head = list->tail = NULL;
    return list;
}

void
dlist_free(dlist *list)
{
    if (list == NULL)
        return;

    struct dlist_n *node = list->head, *next;
    for (; node != NULL; node = next) {
        next = node->next;
        free(node);
    }

    dlist_init(list);
    return;
}

struct dlist_n *
dlist_node(void)
{
    struct dlist_n *node = malloc(sizeof(struct dlist_n));
    if (node == NULL)
        return NULL;

    node->data.ptr = node->prev = node->next = NULL;
    return node;
}

struct dlist_n *
dlist_find_from(dlist *restrict list, struct dlist_n *restrict spos,
                struct dlist_v data)
{
    if (list == NULL || spos == NULL)
        return NULL;

    struct dlist_n *find = spos;
    while (find != NULL) {
        if (find->data.u64 == data.u64)
            return find;
        find = find->next;
    }

    return NULL;
}

struct dlist_n *
dlist_find(dlist *list, struct dlist_v data)
{
    if (list == NULL || list->size == 0)
        return NULL;

    return dlist_find_from(list, list->head, data);
}

struct dlist_n *
dlist_insert(dlist *restrict list, struct dlist_n *restrict pos,
             struct dlist_v data)
{
    if (list == NULL || (pos == NULL && list->size != 0))
        return NULL;

    struct dlist_n *node = dlist_node();
    if (node == NULL)
        return NULL;

    node->data = data;
    node->next = pos;
    if (pos != list->head) {
        node->prev = pos->prev;
        pos->prev->next = node;
        pos->prev = node;
    } else {
        list->head = node;
        list->tail = node;
        pos->prev = node;
    }

    list->size++;
    return node;
}

struct dlist_n *
dlist_push_back(dlist *list, struct dlist_v data)
{
    if (list == NULL)
        return NULL;

    struct dlist_n *node = dlist_node();
    if (node == NULL)
        return NULL;

    node->data = data;
    node->prev = list->tail;
    if (list->size != 0)
        list->tail->next = node;
    else
        list->head = node;

    list->tail = node;
    list->size++;
    return node;
}

struct dlist_n *
dlist_push_front(dlist *list, struct dlist_v data)
{
    if (list == NULL)
        return NULL;

    struct dlist_n *node = dlist_node();
    if (node == NULL)
        return NULL;

    node->data = data;
    node->next = list->head;
    if (list->size != 0)
        list->head->prev = node;
    else
        list->tail = node;

    list->head = node;
    list->size++;
    return node;
}

struct dlist_n *
dlist_erase(dlist *restrict list, struct dlist_n *restrict pos)
{
    if (list == NULL || pos == NULL || list->size == 0)
        return NULL;

    struct dlist_n *prev, *curr, *next;
    prev = pos->prev, curr = pos, next = pos->next;

    free(curr);

    if (list->size == 1)
        list->head = list->tail = NULL;
    else if (pos == list->head)
        list->head = next;
    else if (pos == list->tail)
        list->tail = prev;
    else {
        prev->next = next;
        next->prev = prev;
    }

    list->size--;
    return pos;
}

void
dlist_remove_from(dlist *restrict list, struct dlist_n *restrict spos,
                  struct dlist_v data)
{
    if (list == NULL || spos == NULL || list->size == 0)
        return;

    struct dlist_n *find = dlist_find_from(list, spos, data);
    if (find == NULL)
        return;

    dlist_erase(list, find);
    return;
}

void
dlist_remove(dlist *list, struct dlist_v data)
{
    if (list == NULL || list->size == 0)
        return;

    dlist_remove_from(list, list->head, data);
    return;
}

void
dlist_pop_back(dlist *list)
{
    if (list == NULL || list->size == 0)
        return;

    dlist_erase(list, list->tail);
    return;
}

void
dlist_pop_front(dlist *list)
{
    if (list == NULL || list->size == 0)
        return;

    dlist_erase(list, list->head);
    return;
}
