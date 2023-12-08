#include "../stack.h"
#include <assert.h>

static void test_init(void);
static void test_free(void);
static void test_push(void);
static void test_top(void);
static void test_pop(void);

int main(void)
{
    test_init();
    test_free();
    test_push();
    test_top();
    test_pop();
}

static void test_init(void)
{
    /* init a stack */
    stack stac1;
    stack_init(&stac1);

    assert(stac1.size == 0);
    assert(stac1.capacity == 0);
    assert(stac1.data == NULL);
}

static void test_free(void)
{
    /* free an empty stack */
    stack stac1;
    stack_init(&stac1);

    stack_free(&stac1);

    assert(stac1.size == 0);
    assert(stac1.capacity == 0);
    assert(stac1.data == NULL);

    /* free a stack with some nodes */
    stack stac2;
    stack_init(&stac2);

    struct stack_n node;
    for (int i = 0; i < 100; i++) {
        node.i32 = i;
        stack_push(&stac2, node);
    }

    stack_free(&stac2);

    assert(stac2.size == 0);
    assert(stac2.capacity == 0);
    assert(stac2.data == NULL);
}

static void test_push(void)
{
    /* try to push nodes into stack */
    stack stac1;
    stack_init(&stac1);

    struct stack_n node, *ptr;

    for (int i = 0; i < 1000; i++) {
        node.i32 = i;
        ptr = stack_push(&stac1, node);

        assert(stac1.size == i + 1U);
        assert(ptr->i32 == node.i32);
        assert(ptr == stac1.data + stac1.size - 1);
    }

    assert(stac1.size == 1000);

    stack_free(&stac1);
}

static void test_top(void)
{
    /* NULL will be returned if stack is empty */
    stack stac1;
    stack_init(&stac1);

    assert(stack_top(&stac1) == NULL);

    /* pointer of the top node will be returned if stack has nodes */
    stack stac2;
    stack_init(&stac2);

    struct stack_n node, *ptr;

    for (int i = 0; i < 1000; i++) {
        node.i32 = i;
        ptr = stack_push(&stac2, node);

        assert(stack_top(&stac2) == ptr);
    }

    stack_free(&stac2);
}

static void test_pop(void)
{
    /* an empty stack_n will be returned if stack is empty */
    stack stac1;
    stack_init(&stac1);

    assert(stack_pop(&stac1).ptr == NULL);
    assert(stac1.capacity == 0);
    assert(stac1.data == NULL);
    assert(stac1.size == 0);

    /* a copy of the top node will be returned if stack has nodes */
    stack stac2;
    stack_init(&stac2);

    struct stack_n node, copy;

    for (int i = 0; i < 1000; i++) {
        node.i32 = i;
        stack_push(&stac2, node);
    }

    for (int i = 999; i >= 0; i--) {
        copy = stack_pop(&stac2);

        assert(copy.i32 == i);
        assert(stac2.size == i + 0U);
    }

    assert(stac1.capacity == 0);
    assert(stac1.data == NULL);
    assert(stac1.size == 0);

    stack_free(&stac2);
}
