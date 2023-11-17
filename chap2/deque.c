#include "deque.h"
#include <stdlib.h>

deque *deque_init(deque *que)
{
    que->size = 0;
    que->head = que->tail = NULL;
    return que;
}

void deque_free(deque *que)
{
    struct deque_n *node, *next;
    for (node = que->head; node != NULL; node = next) {
        next = node->next;
        free(node);
    }
    deque_init(que);
    return;
}

struct deque_n *deque_node(void)
{
    struct deque_n *node = malloc(sizeof(struct deque_n));
    node->data.ptr = node->next = NULL;
    return node;
}

struct deque_n *deque_push_end(deque *restrict que,
                               struct deque_n *restrict node)
{
    if (que->size == 0) {
        que->head = node;
        que->tail = node;
    } else {
        que->tail->next = node;
        que->tail = node;
    }

    que->size++;
    return node;
}

struct deque_n *deque_push_head(deque *restrict que,
                                struct deque_n *restrict node)
{
    if (que->size == 0) {
        que->head = node;
        que->tail = node;
    } else {
        node->next = que->head;
        que->head = node;
    }

    que->size++;
    return node;
}

struct deque_n *deque_top(deque *que)
{
    return que->head;
}

struct deque_n deque_pop(deque *que)
{
    struct deque_n ret, *ptr;
    ret.data.ptr = ret.next = NULL;

    if (que->size == 0)
        return ret;

    ptr = deque_top(que);
    ret = *ptr;
    ret.next = NULL;

    struct deque_n *new_head;
    new_head = ptr->next;

    free(ptr);
    que->head = new_head;

    if (--que->size == 0)
        que->tail = NULL;

    return ret;
}