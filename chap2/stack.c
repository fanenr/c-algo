#include "stack.h"
#include <stdlib.h>

stack *
stack_init(stack *stac)
{
    if (stac == NULL)
        return NULL;

    stac->data = NULL;
    stac->size = stac->capacity = 0;
    return stac;
}

stack *
stack_reserve(stack *stac, size_t cap)
{
    if (stac == NULL || cap <= stac->capacity)
        return stac;

    struct stack_n *data = stac->data;
    data = realloc(data, cap * sizeof(struct stack_n));
    if (data == NULL)
        return NULL;

    stac->capacity = cap;
    stac->data = data;
    return stac;
}

void
stack_free(stack *stac)
{
    if (stac == NULL)
        return;

    free(stac->data);
    stack_init(stac);
    return;
}

struct stack_n *
stack_push(stack *stac, struct stack_n node)
{
    if (stac == NULL)
        return NULL;

    size_t cap = stac->capacity;

    if (cap < stac->size + 1) {
        cap = cap == 0 ? STACK_INIT_CAP : cap * STACK_EXPAN_RATIO;
        if (stack_reserve(stac, cap) != stac)
            return NULL;
        stac->capacity = cap;
    }

    struct stack_n *data = stac->data;
    data[stac->size++] = node;
    return data + stac->size - 1;
}

struct stack_n *
stack_top(stack *stac)
{
    if (stac == NULL)
        return NULL;

    size_t off = stac->size != 0 ? stac->size - 1 : 0;
    return stac->data + off;
}

struct stack_n
stack_pop(stack *stac)
{
    struct stack_n ret;
    ret.ptr = NULL;

    if (stac == NULL || stac->size == 0)
        return ret;

    ret = *stack_top(stac);
    stac->size--;
    return ret;
}
