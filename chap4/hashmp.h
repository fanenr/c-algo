/* multi hash map (table) */

#ifndef HASHMP_H
#define HASHMP_H

#include <stddef.h>
#include <stdint.h>

/* hashmp key */
union hashmp_k {
    int64_t i64;
    double f64;
    char *str;
};

/* hashmp value */
union hashmp_v {
    uint8_t u8;
    uint16_t u16;
    uint32_t u32;
    uint64_t u64;

    int8_t i8;
    int16_t i16;
    int32_t i32;
    int64_t i64;

    float f32;
    double f64;

    void *ptr;
    char buf[8];
};

/* hashmp pair */
struct hashmp_p
{
    union hashmp_k key;
    union hashmp_v val;
    struct hashmp_p *prev;
    struct hashmp_p *next;
};

/* hashmp key type */
#define HASHMP_KEY_INT 0
#define HASHMP_KEY_FLT 1
#define HASHMP_KEY_STR 2

/* hashmp initial capacity */
#define HASHMP_INIT_CAP 8
/* hashmp load factor */
#define HASHMP_LOAD_FACTOR 0.8
/* hashmp expand ratio */
#define HASHMP_EXPAN_RATIO 2.0

/* hashmp container (header) */
struct hashmp_s
{
    int ktype;
    size_t size;
    size_t capacity;
    struct hashmp_p **head;
};

typedef struct hashmp_s hashmp;

/*
 * init `map`.
 * A hashmp must have one and only one key `type`, which will be used to compute
 * the hash code.
 */
extern hashmp *hashmp_init(hashmp *map, int type);

/*
 * preset `cap` elements sapce for `map`.
 * it will return directly if `cap` is less than the capacity of `map`.
 */
extern hashmp *hashmp_reserve(hashmp *map, size_t cap);

/*
 * release `map`.
 */
extern void hashmp_free(hashmp *map);

/*
 * allocate a pair.
 */
extern struct hashmp_p *hashmp_pair(void);

/*
 * compute the hash code of `key` (based on the `.ktype` of `map`).
 */
extern long hashmp_hash(hashmp *map, union hashmp_k key);

/*
 * find `key` in `map`.
 * a pointer of the first pair which include `key` will be returned if find,
 * otherwise NULL will be returned.
 */
extern struct hashmp_p *hashmp_find(hashmp *map, union hashmp_k key);

/*
 * find next pair whose key is equal to `pair.key`.
 * `pair` must be in `map` (it can be returned from find).
 */
extern struct hashmp_p *hashmp_find_next(hashmp *restrict map,
                                         struct hashmp_p *restrict pair);

/*
 * insert `pair` into `map`.
 * `pair` will be returned if insert successfully, otherwise NULL will be
 * returned.
 */
extern struct hashmp_p *hashmp_insert(hashmp *restrict map,
                                      struct hashmp_p *restrict pair);

/*
 * remove `pair` in `map`.
 * no pairs will be removed if `pair` is NULL.
 */
extern void hashmp_remove(hashmp *restrict map, struct hashmp_p *restrict pair);

#endif
