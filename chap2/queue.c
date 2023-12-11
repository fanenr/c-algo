#include "queue.h"
#include <stdlib.h>

queue *
queue_init(queue *que)
{
    if (que == NULL)
        return que;

    que->size = 0;
    que->head = NULL;
    return que;
}

void
queue_free(queue *que)
{
    if (que == NULL)
        return;

    struct queue_n *node = que->head, *next;
    for (; node != NULL; node = next) {
        next = node->next;
        free(node);
    }

    queue_init(que);
    return;
}

struct queue_n *
queue_node(void)
{
    struct queue_n *node = malloc(sizeof(struct queue_n));
    if (node == NULL)
        return NULL;

    node->data.ptr = node->next = NULL;
    return node;
}

struct queue_n *
queue_push(queue *que, struct queue_v data)
{
    if (que == NULL)
        return NULL;

    struct queue_n *node = queue_node();
    if (node == NULL)
        return NULL;

    node->next = que->head;
    node->data = data;
    que->head = node;

    que->size++;
    return node;
}

void
queue_pop(queue *que)
{
    if (que == NULL || que->size == 0)
        return;

    struct queue_n *next;
    next = que->head->next;

    free(que->head);

    que->head = next;
    que->size--;
    return;
}
