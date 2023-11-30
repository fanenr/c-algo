/* hash set */

#ifndef HASHST_H
#define HASHST_H

#include <stddef.h>
#include <stdint.h>

/* hashst value */
union hashst_v {
    int64_t i64;
    double f64;
    char *str;
    /* void *obj; */
};

/* hashst value type */
#define HASHST_VAL_INT 0
#define HASHST_VAL_FLT 1
#define HASHST_VAL_STR 2
/* #define HASHST_VAL_OBJ 3 */

/* hashst initial capacity */
#define HASHST_INIT_CAP 8
/* hashst load factor */
#define HASHST_LOAD_FACTOR 0.8
/* hashst expand ratio */
#define HASHST_EXPAN_RATIO 2.0

/* typedef long (*hashst_f)(union hashst_v); */

/* hashst container (header) */
struct hashst_s
{
    int vtype;
    size_t size;
    size_t capacity;
    union hashst_v *head;
};

typedef struct hashst_s hashst;

/*
 * init `set`.
 * A hashst must have one and only one value `type`, which will be used to
 * compute the hash code.
 */
extern hashst *hashst_init(hashst *set, int type);

/*
 * preset `cap` elements sapce for `set`.
 * it will return directly if `cap` is less than the capacity of `set`.
 */
extern hashst *hashst_reserve(hashst *set, size_t cap);

/*
 * release `set`.
 */
extern void hashst_free(hashst *set);

/*
 * compute the hash code of `val` (based on the `.vtype` of `set`).
 */
extern long hashst_hash(hashst *set, union hashst_v val);

/*
 * find `val` in `set`.
 * a pointer of the value which include `val` will be returned if find,
 * otherwise NULL will be returned.
 */
extern union hashst_v *hashst_find(hashst *set, union hashst_v val);

/*
 * insert `val` into `set`.
 * `val` will be returned if insert successfully, otherwise NULL will be
 * returned.
 */
extern union hashst_v *hashst_insert(hashst *set, union hashst_v val);

/*
 * remove `val` in `set`.
 * no values will be removed if `val` is NULL.
 */
extern void hashst_remove(hashst *set, union hashst_v val);

#endif
