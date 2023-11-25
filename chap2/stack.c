#include "stack.h"
#include <stdlib.h>

stack *stack_init(stack *stac)
{
    stac->head = NULL;
    stac->size = stac->capacity = 0;
    return stac;
}

stack *stack_reserve(stack *stac, size_t cap)
{
    if (cap <= stac->capacity)
        return stac;

    struct stack_n *head = stac->head;
    head = realloc(head, cap * sizeof(struct stack_n));
    if (head == NULL)
        return NULL;

    stac->head = head;
    stac->capacity = cap;
    return stac;
}

void stack_free(stack *stac)
{
    free(stac->head);
    stack_init(stac);
    return;
}

struct stack_n *stack_push(stack *stac, struct stack_n node)
{
    size_t cap = stac->capacity;
    struct stack_n *head = stac->head;

    if (cap < stac->size + 1) { /* expand space */
        if (cap == 0)           /* stack is empty */
            cap = STACK_INIT_CAP;
        else                    /* expand stack */
            cap = STACK_EXPAN_RATIO * cap;

        head = realloc(head, cap * sizeof(struct stack_n));
        if (head == NULL)
            return NULL;

        stac->head = head;
        stac->capacity = cap;
    }

    head[stac->size++] = node;
    return head + stac->size - 1;
}

struct stack_n *stack_top(stack *stac)
{
    size_t off = stac->size != 0 ? stac->size - 1 : 0;
    return stac->head + off;
}

struct stack_n stack_pop(stack *stac)
{
    struct stack_n ret;
    ret.ptr = NULL;

    if (stac->size == 0) /* stack is empty */
        return ret;

    ret = *stack_top(stac);
    stac->size--;
    return ret;
}
