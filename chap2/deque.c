#include "deque.h"
#include <stdlib.h>

deque *
deque_init(deque *que)
{
    if (que == NULL)
        return NULL;

    que->size = 0;
    que->head = que->tail = NULL;
    return que;
}

void
deque_free(deque *que)
{
    if (que == NULL)
        return;

    struct deque_n *node = que->head, *next;
    for (; node != NULL; node = next) {
        next = node->next;
        free(node);
    }

    deque_init(que);
    return;
}

struct deque_n *
deque_node(void)
{
    struct deque_n *node = malloc(sizeof(struct deque_n));
    if (node == NULL)
        return NULL;

    node->data.ptr = node->next = node->prev = NULL;
    return node;
}

struct deque_n *
deque_insert(deque *restrict que, struct deque_n *restrict pos,
             struct deque_v data)
{
    if (que == NULL || (pos == NULL && que->size != 0))
        return NULL;

    struct deque_n *node = deque_node();
    if (node == NULL)
        return NULL;

    node->data = data;
    node->next = pos;
    if (pos != que->head) {
        node->prev = pos->prev;
        pos->prev->next = node;
        pos->prev = node;
    } else {
        que->head = node;
        que->tail = node;
        pos->prev = node;
    }

    que->size++;
    return node;
}

struct deque_n *
deque_push_back(deque *que, struct deque_v data)
{
    if (que == NULL)
        return NULL;

    struct deque_n *node = deque_node();
    if (node == NULL)
        return NULL;

    node->data = data;
    node->prev = que->tail;
    if (que->size != 0)
        que->tail->next = node;
    else
        que->head = node;

    que->tail = node;
    que->size++;
    return node;
}

struct deque_n *
deque_push_front(deque *que, struct deque_v data)
{
    if (que == NULL)
        return NULL;

    struct deque_n *node = deque_node();
    if (node == NULL)
        return NULL;

    node->data = data;
    node->next = que->head;
    if (que->size != 0)
        que->head->prev = node;
    else
        que->tail = node;

    que->head = node;
    que->size++;
    return node;
}

struct deque_n *
deque_erase(deque *restrict que, struct deque_n *restrict pos)
{
    if (que == NULL || pos == NULL || que->size == 0)
        return NULL;

    struct deque_n *prev, *next;
    prev = pos->prev, next = pos->next;

    free(pos);

    if (que->size == 1)
        que->head = que->tail = NULL;
    else if (pos == que->head)
        que->head = next;
    else if (pos == que->tail)
        que->tail = prev;
    else {
        prev->next = next;
        next->prev = prev;
    }

    que->size--;
    return pos;
}

void
deque_pop_back(deque *que)
{
    if (que == NULL || que->size == 0)
        return;

    deque_erase(que, que->tail);
    return;
}

void
deque_pop_front(deque *que)
{
    if (que == NULL || que->size == 0)
        return;

    deque_erase(que, que->head);
    return;
}
