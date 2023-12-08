#include "vector.h"
#include <stdlib.h>
#include <string.h>

vector *
vector_init(vector *vec)
{
    if (vec == NULL)
        return NULL;

    vec->data = NULL;
    vec->size = vec->capacity = 0;
    return vec;
}

vector *
vector_reserve(vector *vec, size_t cap)
{
    if (vec == NULL)
        return NULL;

    if (cap <= vec->capacity)
        return vec;

    struct vector_n *data = vec->data;
    data = realloc(data, cap * sizeof(struct vector_n));
    if (data == NULL)
        return NULL;

    vec->capacity = cap;
    vec->data = data;
    return vec;
}

void
vector_free(vector *vec)
{
    if (vec == NULL)
        return;

    free(vec->data);
    vector_init(vec);
    return;
}

struct vector_n *
vector_at(vector *vec, size_t pos)
{
    if (vec == NULL || pos >= vec->size)
        return NULL;

    return vec->data + pos;
}

struct vector_n *
vector_insert(vector *vec, size_t pos, struct vector_n node)
{
    if (vec == NULL || pos > vec->size)
        return NULL;

    size_t cap = vec->capacity;

    if (vec->capacity < vec->size + 1) {
        cap = cap == 0 ? VECTOR_INIT_CAP : cap * VECTOR_EXPAN_RATIO;
        if (vector_reserve(vec, cap) != vec)
            return NULL;
        vec->capacity = cap;
    }

    struct vector_n *data = vec->data;
    size_t mvsiz = (vec->size - pos) * sizeof(struct vector_n);
    struct vector_n *move = memmove(data + pos + 1, data + pos, mvsiz);
    if (move != data + pos + 1)
        return NULL;

    data[pos] = node;
    vec->size++;
    return data + pos;
}

struct vector_n *
vector_push_back(vector *vec, struct vector_n node)
{
    if (vec == NULL)
        return NULL;

    return vector_insert(vec, vec->size, node);
}

struct vector_n *
vector_erase(vector *vec, size_t pos)
{
    if (pos >= vec->size)
        return NULL;

    struct vector_n *move;
    struct vector_n *data = vec->data;

    size_t mvsiz = (vec->size - pos - 1) * sizeof(struct vector_n);
    move = memmove(data + pos, data + pos + 1, mvsiz);
    if (move != data + pos)
        return NULL;

    vec->size--;
    return data + pos;
}
