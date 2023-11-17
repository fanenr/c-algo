#include "queue.h"
#include <stdlib.h>

queue *queue_init(queue *que)
{
    que->size = 0;
    que->head = que->tail = NULL;
    return que;
}

void queue_free(queue *que)
{
    struct queue_n *node, *next;
    for (node = que->head; node != NULL; node = next) {
        next = node->next;
        free(node);
    }
    queue_init(que);
    return;
}

struct queue_n *queue_node(void)
{
    struct queue_n *node = malloc(sizeof(struct queue_n));
    node->data.ptr = node->next = NULL;
    return node;
}

struct queue_n *queue_push(queue *restrict que, struct queue_n *restrict node)
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

struct queue_n *queue_top(queue *que)
{
    return que->head;
}

struct queue_n queue_pop(queue *que)
{
    struct queue_n ret, *ptr;
    ret.data.ptr = ret.next = NULL;

    if (que->size == 0)
        return ret;

    ptr = queue_top(que);
    ret = *ptr;
    ret.next = NULL;

    struct queue_n *new_head;
    new_head = ptr->next;

    free(ptr);
    que->head = new_head;

    if (--que->size == 0)
        que->tail = NULL;

    return ret;
}
