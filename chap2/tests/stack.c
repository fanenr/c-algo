#include "../stack.h"
#include <assert.h>

static void test_init();
static void test_free();
static void test_push();
static void test_top();
static void test_pop();

int main()
{
    test_init();
    test_free();
    test_push();
    test_top();
    test_pop();
}

static void test_init()
{
    /* init a stack */
    stack stac1;
    stack_init(&stac1);

    assert(stac1.size == 0);
    assert(stac1.capacity == 0);
    assert(stac1.head == NULL);

    stack_free(&stac1);
}

static void test_free()
{
    /* free a empty stack */
    stack stac1;
    stack_init(&stac1);

    stack_free(&stac1);

    assert(stac1.size == 0);
    assert(stac1.capacity == 0);
    assert(stac1.head == NULL);

    /* free a stack with some nodes */
    stack stac2;
    stack_init(&stac2);

    struct stack_n node;
    for (int i = 0; i < 100; i++) {
        node.data.i32 = i;
        stack_push(&stac2, node);
    }

    stack_free(&stac2);

    assert(stac2.size == 0);
    assert(stac2.capacity == 0);
    assert(stac2.head == NULL);
}

static void test_push()
{
    /* try to push nodes into stack */
    stack stac1;
    stack_init(&stac1);

    struct stack_n node, *ptr;

    for (int i = 0; i < 1000; i++) {
        node.data.i32 = i;
        ptr = stack_push(&stac1, node);
        assert(ptr->data.i32 == node.data.i32);
    }

    assert(stac1.size == 1000);

    stack_free(&stac1);
}

static void test_top()
{
    /* NULL will be returned if stack is empty */
    stack stac1;
    stack_init(&stac1);

    assert(stack_top(&stac1) == NULL);

    stack_free(&stac1);

    /* pointer of the top node will be returned if stack has nodes */
    stack stac2;
    stack_init(&stac2);

    struct stack_n node, *ptr;

    for (int i = 0; i < 1000; i++) {
        node.data.i32 = i;
        ptr = stack_push(&stac2, node);
        assert(stack_top(&stac2) == ptr);
    }

    stack_free(&stac2);
}

static void test_pop()
{
    /* a empty stack_n will be returned if stack is empty */
    stack stac1;
    stack_init(&stac1);

    assert(stack_pop(&stac1).data.ptr == NULL);

    stack_free(&stac1);

    /* a copy of the top node will be returned if stack has nodes */
    stack stac2;
    stack_init(&stac2);

    struct stack_n node, copy;

    for (int i = 0; i < 1000; i++) {
        node.data.i32 = i;
        stack_push(&stac2, node);
    }

    for (int i = 999; i >= 0; i--) {
        copy = stack_pop(&stac2);
        assert(copy.data.i32 == i);
        assert(stac2.size == i);
    }

    stack_free(&stac2);
}