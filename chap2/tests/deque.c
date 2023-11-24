#include "../deque.h"
#include <assert.h>
#include <stdlib.h>

static void test_init(void);
static void test_free(void);
static void test_node(void);
static void test_push_end(void);
static void test_push_head(void);
static void test_top(void);
static void test_pop(void);

int main(void)
{
    test_top();
    test_pop();
    test_init();
    test_free();
    test_node();
    test_push_end();
    test_push_head();
}

static void test_init(void)
{
    deque que1;
    deque_init(&que1);

    assert(que1.size == 0);
    assert(que1.head == NULL);
    assert(que1.tail == NULL);
}

static void test_free(void)
{
    /* free an empty deque */
    deque que1;
    deque_init(&que1);

    deque_free(&que1);

    assert(que1.size == 0);
    assert(que1.head == NULL);
    assert(que1.tail == NULL);

    /* free a deque with some nodes */
    deque que2;
    deque_init(&que2);

    struct deque_n *node;
    for (int i = 0; i < 100; i++) {
        node = deque_node();
        node->data.i32 = i;
        deque_push_end(&que2, node);
    }

    deque_free(&que2);

    assert(que1.size == 0);
    assert(que1.head == NULL);
    assert(que1.tail == NULL);
}

static void test_node(void)
{
    struct deque_n *node;
    node = deque_node();

    assert(node->next == NULL);
    assert(node->data.ptr == NULL);

    free(node);
}

static void test_push_end(void)
{
    /* try to push some nodes into deque */
    deque que1;
    deque_init(&que1);

    struct deque_n *node, *ptr, *head;

    node = deque_node();
    deque_push_end(&que1, node);
    head = que1.head;

    assert(que1.size == 1);
    assert(que1.head == node);
    assert(que1.tail == node);

    for (int i = 1; i < 1000; i++) {
        node = deque_node();
        node->data.i32 = i;
        ptr = deque_push_end(&que1, node);

        assert(ptr == node);
        assert(que1.tail == ptr);
        assert(que1.head == head);
        assert(que1.size == i + 1U);
        assert(ptr->data.i32 == node->data.i32);
    }

    deque_free(&que1);
}

static void test_push_head(void)
{
    /* try to push some nodes into deque */
    deque que1;
    deque_init(&que1);

    struct deque_n *node, *ptr, *tail;

    node = deque_node();
    deque_push_head(&que1, node);
    tail = que1.tail;

    assert(que1.size == 1);
    assert(que1.head == node);
    assert(que1.tail == node);

    for (int i = 1; i < 1000; i++) {
        node = deque_node();
        node->data.i32 = i;
        ptr = deque_push_head(&que1, node);

        assert(ptr == node);
        assert(que1.head == ptr);
        assert(que1.tail == tail);
        assert(que1.size == i + 1U);
        assert(ptr->data.i32 == node->data.i32);
    }

    deque_free(&que1);
}

static void test_top(void)
{
    /* NULL will be returned if deque is empty */
    deque que1;
    deque_init(&que1);

    assert(deque_top(&que1) == NULL);

    /* pointer of top node will be returned if deque has nodes */
    deque que2;
    deque_init(&que2);

    struct deque_n *node;
    for (int i = 0; i < 1000; i++) {
        node = deque_node();
        node->data.i32 = i;
        deque_push_end(&que2, node);

        assert(deque_top(&que2)->data.i32 == 0);
    }

    deque_free(&que2);
}

static void test_pop(void)
{
    /* an empty deque_n will be returned if deque is empty */
    deque que1;
    deque_init(&que1);

    assert(deque_pop(&que1).data.ptr == NULL);
    assert(que1.head == NULL);
    assert(que1.tail == NULL);
    assert(que1.size == 0);

    /* a copy of the top node will be returned if deque has nodes */
    deque que2;
    deque_init(&que2);

    struct deque_n *node, copy;
    for (int i = 0; i < 1000; i++) {
        node = deque_node();
        node->data.i32 = i;
        deque_push_end(&que2, node);
    }

    for (int i = 0; i < 1000; i++) {
        copy = deque_pop(&que2);

        assert(copy.next == NULL);
        assert(copy.data.i32 == i);
        assert(que2.size == 999U - i);
    }

    assert(que2.size == 0);
    assert(que2.head == NULL);
    assert(que2.tail == NULL);

    deque_free(&que2);
}
