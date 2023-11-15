#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

stack *stack_init(stack *stac)
{
    stac->head = NULL;
    stac->size = stac->capacity = 0;
    return stac;
}

void stack_free(stack *stac)
{
    free(stac->head);
    stack_init(stac);
    return;
}

struct stack_n *stack_push(stack *restrict stac, struct stack_n node)
{
    if (stac->size < stac->capacity) /* sapce is enough */
        goto push;

    /* reallocate space to hold new node */
    struct stack_n *new_head;
    size_t new_cap = stac->capacity;

    if (new_cap == 0) /* stack is empty */
        new_cap = STACK_INIT_CAP;
    else              /* expand stack */
        new_cap = STACK_EXPAN_RATIO * new_cap;

    new_head = realloc(stac->head, new_cap * sizeof(struct stack_n));
    if (new_head == NULL)
        return NULL;

    stac->head = new_head;
    stac->capacity = new_cap;

push:
    stac->head[stac->size++] = node;

    return stac->head + stac->size - 1;
}

struct stack_n *stack_top(stack *stac)
{
    if (stac->size == 0)
        return NULL;
    return stac->head + stac->size - 1;
}

struct stack_n stack_pop(stack *stac)
{
    struct stack_n ret, *ptr;
    ret.data.ptr = NULL;

    if (stac->size == 0) /* stack is empty */
        return ret;

    ptr = stack_top(stac);
    ret = *ptr;

    stac->size--;
    return ret;
}
