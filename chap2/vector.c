#include "vector.h"
#include <stdlib.h>
#include <string.h>

vector *vector_init(vector *vec)
{
    vec->head = NULL;
    vec->size = vec->capacity = 0;
    return vec;
}

vector *vector_reserve(vector *vec, size_t cap)
{
    if (cap <= vec->capacity)
        return vec;

    struct vector_n *head = vec->head;
    head = realloc(head, cap * sizeof(struct vector_n));
    if (head == NULL)
        return NULL;

    vec->head = head;
    vec->capacity = cap;
    return vec;
}

void vector_free(vector *vec)
{
    free(vec->head);
    vector_init(vec);
    return;
}

struct vector_n *vector_get(vector *vec, size_t pos)
{
    if (pos >= vec->size)
        return NULL;
    return vec->head + pos;
}

struct vector_n *vector_insert(vector *vec, size_t pos, struct vector_n node)
{
    if (pos >= vec->size)
        return NULL;

    size_t cap = vec->capacity;
    struct vector_n *head = vec->head;

    if (vec->capacity < vec->size + 1) { /* expand sapce */
        if (cap == 0)                    /* vector is empty */
            cap = VECTOR_INIT_CAP;
        else                             /* expand vector */
            cap = VECTOR_EXPAN_RATIO * cap;

        head = realloc(head, cap * sizeof(struct vector_n));
        if (head == NULL)
            return NULL;

        vec->head = head;
        vec->capacity = cap;
    }

    struct vector_n *move;
    move = memmove(head + pos + 1, head + pos, vec->size - pos);
    if (move != head + pos + 1)
        return NULL;

    head[pos] = node;
    vec->size++;
    return head + pos;
}

struct vector_n *vector_push_end(vector *vec, struct vector_n node)
{
    return vector_insert(vec, vec->size - 1, node);
}

struct vector_n *vector_remove(vector *vec, size_t pos)
{
    if (pos >= vec->size)
        return NULL;

    struct vector_n *move;
    struct vector_n *head = vec->head;

    move = memmove(head + pos, head + pos + 1, vec->size - pos - 1);
    if (move != head + pos)
        return NULL;

    vec->size--;
    return head + pos;
}
