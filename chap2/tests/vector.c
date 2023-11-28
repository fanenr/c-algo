#include "../vector.h"
#include <assert.h>

static void test_init(void);
static void test_reserve(void);
static void insert_helper(vector *vec);
static void test_free(void);
static void test_get(void);
static void test_insert(void);
static void test_push_end(void);
static void test_remove(void);

int main(void)
{
    test_init();
    test_reserve();
    test_free();
    test_get();
    test_insert();
    test_push_end();
    test_remove();
    return 0;
}

static void test_init(void)
{
    vector vec1;
    vector_init(&vec1);

    assert(vec1.size == 0);
    assert(vec1.head == NULL);
    assert(vec1.capacity == 0);
}

static void test_reserve(void)
{
    vector vec1;
    vector_init(&vec1);

    /* preset 4 elements space for vector */
    vector_reserve(&vec1, 4);

    assert(vec1.capacity == 4);

    vector_free(&vec1);

    vector vec2;
    vector_init(&vec2);

    /* preset 100 elements space for vector */
    vector_reserve(&vec2, 100);

    assert(vec2.capacity == 100);

    vector_free(&vec2);
}

static void insert_helper(vector *vec)
{
    struct vector_n node, *pos;

    for (int i = 0; i < 100; i++) {
        node.i32 = i;
        pos = vector_push_end(vec, node);

        assert(vec->size == i + 1U);
        assert(pos == vec->head + i);
    }
}

static void test_free(void)
{
    /* release a empty vector */
    vector vec1;
    vector_init(&vec1);

    vector_free(&vec1);

    assert(vec1.size == 0);
    assert(vec1.head == NULL);
    assert(vec1.capacity == 0);

    /* release a vector with some nodes */
    vector vec2;
    vector_init(&vec2);

    insert_helper(&vec2);

    vector_free(&vec2);

    assert(vec2.size == 0);
    assert(vec2.head == NULL);
    assert(vec2.capacity == 0);
}

static void test_get(void)
{
    vector vec1;
    vector_init(&vec1);

    insert_helper(&vec1);

    struct vector_n *node;
    for (int i = 0; i < 100; i++) {
        node = vector_get(&vec1, i);

        assert(node->i32 == i);
        assert(node == vec1.head + i);
    }

    vector_free(&vec1);
}

static void test_insert(void)
{
    vector vec1;
    vector_init(&vec1);

    struct vector_n node, *pos;
    for (int i = 99; i >= 0; i--) {
        node.i32 = i;
        pos = vector_insert(&vec1, 0, node);

        assert(pos == vec1.head);
        assert(vec1.size == 100U - i);
    }

    for (int i = 0; i < 100; i++) {
        pos = vector_get(&vec1, i);

        assert(pos->i32 == i);
        assert(pos == vec1.head + i);
    }

    vector_free(&vec1);
}

static void test_push_end(void)
{
    vector vec1;
    vector_init(&vec1);

    insert_helper(&vec1);

    vector_free(&vec1);
}

static void test_remove(void)
{
    vector vec1;
    vector_init(&vec1);

    insert_helper(&vec1);

    struct vector_n *pos;
    for (int i = 0; i < 99; i++) {
        pos = vector_remove(&vec1, 0);

        assert(pos == vec1.head);
        assert(pos->i32 == i + 1);
        assert(vec1.size == 99U - i);
    }

    assert(pos->i32 == 99);
    assert(vec1.size == 1);

    /* must release */
    vector_free(&vec1);
}
