#include "hashst.h"
#include <stdlib.h>
#include <string.h>

hashst *hashst_init(hashst *set, int type)
{
    set->head = NULL;
    set->vtype = type;
    set->size = set->capacity = 0;
    return set;
}

extern hashst *hashst_reserve(hashst *set, size_t cap)
{
    if (cap <= set->capacity)
        return set;

    union hashst_v *head = calloc(cap, sizeof(union hashst_v));
    if (head == NULL)
        return NULL;

    union hashst_v *dest = memcpy(head, set->head, set->capacity);
    if (dest != head) {
        free(head);
        return NULL;
    }

    free(set->head);
    set->head = head;
    set->capacity = cap;

    return set;
}

void hashst_free(hashst *set)
{
    free(set->head);
    hashst_init(set, set->vtype);
}

static size_t hashst_hash_sdbm(const char *str)
{
    if (str == NULL)
        return 0;
    size_t hash = 0;
    for (size_t i = 0; str[i] != 0; i++)
        hash = str[i] + (hash << 6) + (hash << 16) - hash;
    return hash;
}

long hashst_hash(hashst *set, union hashst_v val)
{
    if (set->capacity == 0)
        return -1;

    long hash = -1;
    int type = set->vtype;

    switch (type) {
    case HASHST_VAL_INT:
        hash = (uint64_t)val.i64 % set->capacity;
        break;

    case HASHST_VAL_FLT:
        hash = (uint64_t)val.i64 % set->capacity;
        break;

    case HASHST_VAL_STR:
        hash = hashst_hash_sdbm(val.str) % set->capacity;
        break;

    default:
        return -1;
    }

    return hash;
}

union hashst_v *hashst_find(hashst *set, union hashst_v val)
{
    int type = set->vtype;
    long hash = hashst_hash(set, val);

    if (hash < 0)
        return NULL;

    union hashst_v *find = set->head + hash;

    switch (type) {
    case HASHST_VAL_INT:
        if (val.i64 == find->i64)
            return find;
        break;

    case HASHST_VAL_FLT:
        if (val.f64 == find->f64)
            return find;
        break;

    case HASHST_VAL_STR:
        if (val.str == find->str)
            return find;
        if (find->str == NULL || val.str == NULL)
            break;
        if (strcmp(val.str, find->str) == 0)
            return find;
        break;

    default:
        return NULL;
    }

    return NULL;
}

union hashst_v *hashst_insert(hashst *set, union hashst_v val)
{
    size_t cap = set->capacity;

    if (cap * HASHST_LOAD_FACTOR < set->size + 1) { /* expand sapce */
        if (cap == 0)                               /* set is empty */
            cap = HASHST_INIT_CAP;
        else                                        /* expand map */
            cap = cap * HASHST_EXPAN_RATIO;

        if (hashst_reserve(set, cap) != set) /* expand failed */
            return NULL;
    }

    long hash = hashst_hash(set, val);

    if (hash < 0)
        return NULL;

    set->head[hash] = val;
    set->size++;

    return set->head + hash;
}

void hashst_remove(hashst *set, union hashst_v val)
{
    union hashst_v *find = hashst_find(set, val);
    if (find == NULL)
        return;

    find->str = NULL;
    set->size--;
    return;
}
