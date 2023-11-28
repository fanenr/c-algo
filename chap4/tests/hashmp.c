#include "../hashmp.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

static void test_init(void);
static void test_reserve(void);
static void test_free(void);
static void test_pair(void);
static void test_hash(void);
static void test_find(void);
static void test_insert(void);
static void test_remove(void);

#define build_pair(_key_t, _key, _val_t, _val)                                 \
    ({                                                                         \
        struct hashmp_p *_ret = hashmp_pair();                                 \
        _ret->key._key_t = _key;                                               \
        _ret->val._val_t = _val;                                               \
        _ret;                                                                  \
    })

#define build_key(_key_t, _key)                                                \
    ({                                                                         \
        union hashmp_k _ret;                                                   \
        _ret._key_t = _key;                                                    \
        _ret;                                                                  \
    })

#define build_val(_val_t, _val)                                                \
    ({                                                                         \
        union hashmp_v _ret;                                                   \
        _ret._val_t = _val;                                                    \
        _ret;                                                                  \
    })

int main(void)
{
    test_init();
    test_reserve();
    test_free();
    test_pair();
    test_hash();
    test_find();
    test_insert();
    test_remove();
}

static void test_init(void)
{
    hashmp map1;
    hashmp_init(&map1, HASHMP_KEY_INT);

    assert(map1.size == 0);
    assert(map1.head == NULL);
    assert(map1.capacity == 0);
    assert(map1.ktype == HASHMP_KEY_INT);
}

static void test_reserve(void)
{
    hashmp map1;
    hashmp_init(&map1, HASHMP_KEY_INT);

    hashmp_reserve(&map1, 8);

    assert(map1.head != NULL);
    assert(map1.capacity == 8);

    hashmp_reserve(&map1, 100);

    assert(map1.head != NULL);
    assert(map1.capacity == 100);

    hashmp_free(&map1);
}

static void test_free(void)
{
    /* free a emtpy hashmp */
    hashmp map1;
    hashmp_init(&map1, HASHMP_KEY_INT);

    hashmp_free(&map1);

    assert(map1.size == 0);
    assert(map1.head == NULL);
    assert(map1.capacity == 0);
    assert(map1.ktype == HASHMP_KEY_INT);

    /* free a hashmp with some pairs */
    hashmp map2;
    hashmp_init(&map2, HASHMP_KEY_STR);

    hashmp_insert(&map2, build_pair(str, "Hello", i32, 10));
    hashmp_free(&map2);

    assert(map2.size == 0);
    assert(map2.head == NULL);
    assert(map2.capacity == 0);
    assert(map2.ktype == HASHMP_KEY_STR);
}

static void test_pair(void)
{
    struct hashmp_p *pair;
    pair = hashmp_pair();

    assert(pair->key.i64 == 0);
    assert(pair->val.i64 == 0);
    assert(pair->prev == NULL);
    assert(pair->next == NULL);

    free(pair);
}

static size_t hashmp_hash_sdbm(const char *str)
{
    size_t hash = 0;
    for (size_t i = 0; str[i] != 0; i++)
        hash = str[i] + (hash << 6) + (hash << 16) - hash;
    return hash;
}

static void test_hash(void)
{
    /* test i64 key hash */
    hashmp map1;
    hashmp_init(&map1, HASHMP_KEY_INT);

    /* capacity is 0 */
    assert(hashmp_hash(&map1, build_key(i64, 10)) == -1);

    hashmp_reserve(&map1, HASHMP_INIT_CAP);

    assert(hashmp_hash(&map1, build_key(i64, 11)) == 11 % 8);

    hashmp_free(&map1);

    /* test f64 key hash */
    hashmp map2;
    hashmp_init(&map2, HASHMP_KEY_FLT);

    /* capacity is 0 */
    assert(hashmp_hash(&map2, build_key(f64, 3.14)) == -1);

    hashmp_reserve(&map2, HASHMP_INIT_CAP);

    assert(hashmp_hash(&map2, build_key(f64, 3.14)) == (uint64_t)3.14 % 8);

    hashmp_free(&map2);

    /* test str key hash */
    hashmp map3;
    hashmp_init(&map3, HASHMP_KEY_STR);

    /* capacity is 0 */
    assert(hashmp_hash(&map3, build_key(str, "Hello World")) == -1);

    hashmp_reserve(&map3, HASHMP_INIT_CAP);

    assert((size_t)hashmp_hash(&map3, build_key(str, "Hello World")) ==
           hashmp_hash_sdbm("Hello World") % 8);

    hashmp_free(&map3);
}

static void test_find(void)
{
    hashmp map1;
    hashmp_init(&map1, HASHMP_KEY_STR);

    struct hashmp_p *pair, *next;

    hashmp_insert(&map1, build_pair(str, "Tom", i32, 10));
    hashmp_insert(&map1, build_pair(str, "Tom", i32, 11));
    hashmp_insert(&map1, build_pair(str, "Tom", i32, 12));

    pair = hashmp_find(&map1, build_key(str, "Tom"));

    assert(pair->prev == NULL);
    assert(pair->val.i32 == 10);

    /* find_next */
    next = hashmp_find_next(&map1, pair);
    assert(pair->next == next);
    assert(next->prev == pair);

    pair = next;
    assert(pair->val.i32 == 11);

    next = hashmp_find_next(&map1, pair);
    assert(next->prev == pair);
    assert(pair->next == next);
    assert(next->next == NULL);
    assert(next->val.i32 == 12);

    hashmp_free(&map1);
}

static void test_insert(void)
{
    hashmp map1;
    hashmp_init(&map1, HASHMP_KEY_STR);

    struct hashmp_p *pair;

    pair = build_pair(str, "Tom", i32, 10);

    assert(hashmp_insert(&map1, pair) == pair);
    assert(pair->prev == NULL);
    assert(pair->next == NULL);
    assert(map1.size == 1);

    pair = build_pair(str, "Tom", i32, 11);

    assert(hashmp_insert(&map1, pair) == pair);
    assert(pair->prev->val.i32 == 10);
    assert(pair->next == NULL);
    assert(map1.size == 2);

    hashmp_free(&map1);
}

static void test_remove(void)
{
    hashmp map1;
    hashmp_init(&map1, HASHMP_KEY_STR);

    struct hashmp_p *pair, *next;

    hashmp_insert(&map1, build_pair(str, "Tom", i32, 10));
    hashmp_insert(&map1, build_pair(str, "Tom", i32, 11));
    hashmp_insert(&map1, build_pair(str, "Tom", i32, 12));

    pair = hashmp_find(&map1, build_key(str, "Tom"));
    hashmp_remove(&map1, pair->next);
    next = hashmp_find_next(&map1, pair);

    assert(map1.size == 2);
    assert(pair->next == next);
    assert(pair->prev == NULL);
    assert(next->prev == pair);
    assert(next->next == NULL);
    assert(pair->val.i32 == 10);
    assert(next->val.i32 == 12);

    hashmp_remove(&map1, pair);
    pair = hashmp_find(&map1, build_key(str, "Tom"));
    next = hashmp_find_next(&map1, pair);

    assert(pair->val.i32 == 12);
    assert(pair->prev == NULL);
    assert(pair->next == NULL);
    assert(map1.size == 1);
    assert(next == NULL);
    assert(map1.head[hashmp_hash(&map1, build_key(str, "Tom"))] == pair);

    hashmp_free(&map1);
}
