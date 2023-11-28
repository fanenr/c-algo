#include "hashmp.h"
#include <stdlib.h>
#include <string.h>

hashmp *hashmp_init(hashmp *map, int type)
{
    map->head = NULL;
    map->ktype = type;
    map->size = map->capacity = 0;
    return map;
}

hashmp *hashmp_reserve(hashmp *map, size_t cap)
{
    if (cap <= map->capacity)
        return map;

    struct hashmp_p **head = calloc(cap, sizeof(struct hashmp_p *));
    if (head == NULL)
        return NULL;

    struct hashmp_p **dest =
        memcpy(head, map->head, map->capacity * sizeof(struct hashmp_p *));
    if (dest != head) {
        free(head);
        return NULL;
    }

    free(map->head);
    map->head = head;
    map->capacity = cap;

    return map;
}

void hashmp_free(hashmp *map)
{
    if (map->head != NULL) {
        struct hashmp_p *pair, *next;
        for (size_t i = 0; i < map->capacity; i++)
            for (pair = map->head[i]; pair != NULL;) {
                next = pair->next;
                free(pair);
                pair = next;
            }
    }

    free(map->head);
    hashmp_init(map, map->ktype);
    return;
}

struct hashmp_p *hashmp_pair(void)
{
    struct hashmp_p *pair = malloc(sizeof(struct hashmp_p));
    pair->key.str = pair->val.ptr = pair->prev = pair->next = NULL;
    return pair;
}

static size_t hashmp_hash_sdbm(const char *str)
{
    size_t hash = 0;
    for (size_t i = 0; str[i] != 0; i++)
        hash = str[i] + (hash << 6) + (hash << 16) - hash;
    return hash;
}

long hashmp_hash(hashmp *map, union hashmp_k key)
{
    if (map->capacity == 0)
        return -1;

    long hash;
    int type = map->ktype;
    size_t cap = map->capacity;

    switch (type) {
    case HASHMP_KEY_INT:
        hash = (uint64_t)key.i64 % cap;
        break;
    case HASHMP_KEY_FLT:
        hash = (uint64_t)key.f64 % cap;
        break;
    case HASHMP_KEY_STR:
        hash = hashmp_hash_sdbm(key.str) % cap;
        break;
    default:
        return -1;
    }

    return hash;
}

struct hashmp_p *hashmp_find(hashmp *map, union hashmp_k key)
{
    int type = map->ktype;
    long hash = hashmp_hash(map, key);

    if (hash < 0)
        return NULL;

    struct hashmp_p *pair = map->head[hash];

    switch (type) {
    case HASHMP_KEY_INT:
        for (; pair != NULL; pair = pair->next)
            if (key.i64 == pair->key.i64)
                break;
        break;
    case HASHMP_KEY_FLT:
        for (; pair != NULL; pair = pair->next)
            if (key.f64 == pair->key.f64)
                break;
        break;
    case HASHMP_KEY_STR:
        for (; pair != NULL; pair = pair->next)
            if (strcmp(key.str, pair->key.str) == 0)
                break;
        break;
    default:
        return NULL;
    }

    return pair;
}

struct hashmp_p *hashmp_find_next(hashmp *map, struct hashmp_p *pair)
{
    if (pair == NULL)
        return NULL;

    int type = map->ktype;
    struct hashmp_p *next = pair->next;

    switch (type) {
    case HASHMP_KEY_INT:
        for (; next != NULL; next = next->next)
            if (next->key.i64 == pair->key.i64)
                break;
        break;
    case HASHMP_KEY_FLT:
        for (; next != NULL; next = next->next)
            if (next->key.f64 == pair->key.f64)
                break;
        break;
    case HASHMP_KEY_STR:
        for (; next != NULL; next = next->next)
            if (strcmp(next->key.str, pair->key.str) == 0)
                break;
        break;
    default:
        return NULL;
    }

    return next;
}

struct hashmp_p *hashmp_insert(hashmp *map, struct hashmp_p *pair)
{
    size_t cap = map->capacity;

    if (cap * HASHMP_LOAD_FACTOR < map->size + 1) { /* expand sapce */
        if (cap == 0)                               /* map is empty */
            cap = HASHMP_INIT_CAP;
        else                                        /* expand map */
            cap = cap * HASHMP_EXPAN_RATIO;

        if (hashmp_reserve(map, cap) != map) /* expand failed */
            return NULL;
    }

    long hash = hashmp_hash(map, pair->key);
    if (hash < 0)
        return NULL;

    struct hashmp_p *prev = map->head[hash];
    if (prev == NULL)
        map->head[hash] = pair;
    else {
        while (prev->next != NULL)
            prev = prev->next;
        pair->prev = prev;
        prev->next = pair;
    }

    map->size++;
    return pair;
}

void hashmp_remove(hashmp *map, struct hashmp_p *pair)
{
    struct hashmp_p *find = hashmp_find(map, pair->key);

    for (; find != NULL; find = find->next)
        if (find == pair)
            break;

    if (find == NULL)
        return;

    long hash = hashmp_hash(map, find->key);
    struct hashmp_p *prev, *next;
    prev = find->prev;
    next = find->next;
    free(find);

    if (prev == NULL) /* remove head[hash] */
        map->head[hash] = next;
    else
        prev->next = next;

    if (next != NULL)
        next->prev = prev;

    map->size--;
    return;
}
